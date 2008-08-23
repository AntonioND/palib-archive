// Includes
#include <PA9.h>

int pa_transferScore(char *buffer, char *adress);
int pa_httpServer(char *buffer, char *buffer2);
int pa_wordSearch(char *mot1, char *mot2, int depart);


//Ideally - you shouldn't need to change any of this code.
bool PA_SubmitScore(char *response, char *gamekey, char *playername, int score, char *level, u8 responsetype)
{
	bool returnval = true;
   if (PA_ConnectWifiWFC())
   {
   	PA_WaitForVBL();
	  	char url[256];
		sprintf(url, "http://leaderboard.palib.info/submitscore.php?gamekey=%s&playername=%s&levelname=%s&score=%d&responsetype=%d&end", gamekey, playername, level, score, responsetype);

		pa_transferScore(response, url);
	}		
	else
	{
	   returnval = false;
	} 
	PA_WaitForVBL();
	Wifi_DisconnectAP();
	PA_WaitForVBL();
	Wifi_DisableWifi();  
	return returnval;
 }   
 
//modified version of: int PA_GetHTTP(char *buffer, char *adress);
int pa_transferScore(char *buffer, char *adress)
{
    int sock;
    char serveur[256];
    char buffer3[256];
    int pos = pa_httpServer(serveur,adress);
    PA_InitSocket(&sock,serveur,80,PA_NONBLOCKING_TCP);
    char buffer2[256];
    sprintf(buffer2, "GET %s HTTP/1.0\r\nhost: %s\r\nAccept: */*\r\n\r\n",adress+pos,serveur);
    send(sock,buffer2,256,0);
    strcpy(buffer,"");
 
    while(pa_wordSearch(buffer3,"\r\n\r\n",0) == -1)
    {
      if (Pad.Newpress.Start) break;
	    memset(buffer3,0,sizeof(buffer3));
       recv(sock,buffer3,256,0);
    }
    int poshtml = pa_wordSearch(buffer3,"\r\n\r\n",0)+4;
    strcat(buffer,buffer3+poshtml);
    while(1)//wordSearch(buffer,"-----",0) == -1)
    {
      if (Pad.Newpress.Start) break;
    	memset(buffer3,0,sizeof(buffer3));
    	if(recv(sock,buffer3,256,0)<1) break;
	   strcat(buffer,buffer3);
    }
    if(sock) closesocket(sock);
	return 1;
}
 
 
 //modified version of: int get_HTTP_serveurx(char *buffer, char *buffer2);
 int pa_httpServer(char *buffer, char *buffer2)
{
    int i,depart=0;
    if(pa_wordSearch(buffer2,"http://",0)!=-1)
       depart+=7;
    for(i = depart; buffer2[i] != '\0' && buffer2[i] != '/' && buffer2[i] != '\\'; i++)
    {
          buffer[i-depart]=buffer2[i];
    }
    buffer[i-depart]='\0';
return i;
}
 
 
//modified version of: int search_word(char *mot1, char *mot2, int depart);
int pa_wordSearch(char *mot1, char *mot2, int depart){
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

