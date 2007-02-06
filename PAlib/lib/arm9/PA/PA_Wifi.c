#include <PA9.h>

#include <arm9/PA_Wifi.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef FD_SETSIZE
	#undef FD_SETSIZE
#endif
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>



#define isdigit(c) (c>='0'&&c<='9')
#define IS_DIGIT_OR_DOT(c) (isdigit(c)||(c=='.'))
#define IS_INETADDR(s) (IS_DIGIT_OR_DOT(s[0])&&IS_DIGIT_OR_DOT(s[1])&&IS_DIGIT_OR_DOT(s[2])&&IS_DIGIT_OR_DOT(s[3])&&IS_DIGIT_OR_DOT(s[4])&&IS_DIGIT_OR_DOT(s[5])&&IS_DIGIT_OR_DOT(s[6]))

// Dswifi stub functions


// sgIP_dbgprint only needed in debug version
void sgIP_dbgprint(char * txt, ...) {		
}

// wifi timer function, to update internals of sgIP
void Timer_50ms(void) {
   Wifi_Timer(50);
   //PA_OutputText(0, 0, 0, "VBL Ok");
  
//PA_OutputText(0, 0, 10, "%d", PA_nVBLs);
}

// notification function to send fifo message to arm7
void arm9_synctoarm7() { // send fifo message
   REG_IPC_FIFO_TX=0x87654321;

}

// interrupt handler to receive fifo messages from arm7
void arm9_fifo() { // check incoming fifo messages
   u32 value = REG_IPC_FIFO_RX;
   if(value == 0x87654321) Wifi_Sync();

}

void PA_InitWifi() {
	
	REG_IPC_FIFO_CR = IPC_FIFO_ENABLE | IPC_FIFO_SEND_CLEAR; // enable & clear FIFO
		
	u32 Wifi_pass= Wifi_Init(WIFIINIT_OPTION_USELED);
   	REG_IPC_FIFO_TX=0x12345678;
   	REG_IPC_FIFO_TX=Wifi_pass;
   	
	*((volatile u16 *)0x0400010E) = 0; // disable timer3
		
	//irqInit(); 
	irqSet(IRQ_TIMER3, Timer_50ms); // setup timer IRQ
	irqEnable(IRQ_TIMER3);
   	irqSet(IRQ_FIFO_NOT_EMPTY, arm9_fifo); // setup fifo IRQ
   	irqEnable(IRQ_FIFO_NOT_EMPTY);
   	
   	REG_IPC_FIFO_CR = IPC_FIFO_ENABLE | IPC_FIFO_RECV_IRQ; // enable FIFO IRQ
   	
   	Wifi_SetSyncHandler(arm9_synctoarm7); // tell wifi lib to use our handler to notify arm7

	// set timer3
	*((volatile u16 *)0x0400010C) = -6553; // 6553.1 * 256 cycles = ~50ms;
	*((volatile u16 *)0x0400010E) = 0x00C2; // enable, irq, 1/256 clock
	
	while(Wifi_CheckInit()==0) { // wait for arm7 to be initted successfully
		while(VCOUNT>192); // wait for vblank
		while(VCOUNT<192);
	}
		
}

bool PA_ConnectWifiWFC() {
	int i;
	
	Wifi_AutoConnect(); // request connect
	
	while(1) {
		i=Wifi_AssocStatus(); // check status
		if(i==ASSOCSTATUS_ASSOCIATED) {
			return true;
		}
		if(i==ASSOCSTATUS_CANNOTCONNECT) {
			return false;
		}
	}
}

unsigned long makelong(unsigned char data1, unsigned char data2,unsigned char data3,unsigned char data4)
{
    return (data4<<24) | (data3<<16) | (data2<<8) | data1;  
}

unsigned long PA_chartoip(char *message)
{
	int i,j;
	unsigned char data[4];
	int debut=0;
	
	for(j=0;j<4;j++)
	{
		char buffer[256];
		for(i=debut;message[i]!='.'&&message[i]!='\0';i++)
		{
		   buffer[i-debut]=message[i];
		}
		buffer[i-debut]='\0';
		debut=i+1;
		data[j]=atoi(buffer);
	}
	return makelong(data[0],data[1],data[2],data[3]);
}

int PA_InitSocket(int *sock,char *host,int port,int mode)
{
     unsigned long ip;
     struct     sockaddr_in    servaddr;   
     *sock = socket(AF_INET, SOCK_STREAM, 0);
     if(IS_INETADDR(host))
       ip = PA_chartoip(host);
     else
       ip = *(unsigned long *)gethostbyname(host)->h_addr_list[0];
       
     servaddr.sin_family = AF_INET;
     servaddr.sin_port = htons(port);     
     servaddr.sin_addr.s_addr = ip;
     
     if(mode == PA_NORMAL_TCP)
     {
     if(connect(*sock, (struct sockaddr *) &servaddr, sizeof(servaddr))==0)
        return 1;
     }
     else if(mode == PA_NONBLOCKING_TCP)
     {
     if(connect(*sock, (struct sockaddr *) &servaddr, sizeof(servaddr))==0)
        {
        int i = 1;
        ioctl(*sock, FIONBIO, &i);
        return 1;
        }
     }
     
        
return 0;
}



int PA_InitServer(int *sock,int port,int mode, int num_connect)
{
    int i;
     struct     sockaddr_in    servaddr;   
     *sock = socket(AF_INET, SOCK_STREAM, 0);
       
     servaddr.sin_family = AF_INET;
     servaddr.sin_port = htons(port);     
     servaddr.sin_addr.s_addr = 0;
     
     if(mode == PA_NORMAL_TCP)
     {
        bind(*sock,(struct sockaddr *)&servaddr,sizeof(servaddr));
        listen(*sock,num_connect);
        return 1;
     }
     else if(mode == PA_NONBLOCKING_TCP)
     {
        bind(*sock,(struct sockaddr *)&servaddr,sizeof(servaddr));
        i = 1;
        ioctl(*sock, FIONBIO, &i);
        listen(*sock,num_connect);
        return 1;
     }
/*
        while(!sock)
        {
           sock=accept(*sock,(struct sockaddr *)&servaddr,&i);
        }
*/
       
return 0;
}

int search_word(char *mot1, char *mot2, int depart){
	int i,j,erreur=1;
	for(i=depart;i<strlen(mot1);i++){
	
		if(mot1[i]==mot2[0]){
			erreur=0;
			
			for(j=0;j<strlen(mot2);j++) 
				if(mot2[j]!=mot1[i+j]) 
					erreur=1;
					
			if(erreur==0)
				return i;
		}
	}
	return -1;
}



int get_HTTP_serveur(char *buffer, char *buffer2)
{
    int i,depart=0;
    if(search_word(buffer2,"http://",0)!=-1)
       depart+=7;
    for(i = depart; buffer2[i] != '\0' && buffer2[i] != '/' && buffer2[i] != '\\'; i++)
    {
          buffer[i-depart]=buffer2[i];
    }
    buffer[i-depart]='\0';
return i;
}



int PA_GetHTTP(char *buffer, char *adress)
{
    int sock;
    char serveur[256];
    char buffer3[256];
    int pos = get_HTTP_serveur(serveur,adress);
    PA_InitSocket(&sock,serveur,80,PA_NONBLOCKING_TCP);
    char buffer2[256];
    sprintf(buffer2, "GET %s HTTP/1.1\r\nhost: %s\r\nAccept: */*\r\n\r\n",adress+pos,serveur);
    send(sock,buffer2,256,0);
    strcpy(buffer,"");
    while(search_word(buffer3,"\r\n\r\n",0) == -1)
    {
       memset(buffer3,0,sizeof(buffer3));
       recv(sock,buffer3,256,0);
    }
    int poshtml = search_word(buffer3,"\r\n\r\n",0)+4;
    strcat(buffer,buffer3+poshtml);
    while(search_word(buffer,"</html>",0) == -1 && search_word(buffer,"</HTML>",0) == -1)
    {
    memset(buffer3,0,sizeof(buffer3));
    if(recv(sock,buffer3,256,0)<1)
       break;
    strcat(buffer,buffer3);
    }
    if(sock)
       closesocket(sock);
return 1;
}





