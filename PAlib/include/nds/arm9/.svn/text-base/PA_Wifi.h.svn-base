#include <dswifi9.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define		VCOUNT		(*((u16 volatile *) 0x04000006))

#define IPADDR(d,c,b,a) (unsigned long)(((( a )&0xff)<<24)|((( b )&0xff)<<16)|((( c )&0xff)<<8 )|((( d )&0xff)))

#define PA_NORMAL_TCP 1
#define PA_NONBLOCKING_TCP 2

extern void * sgIP_malloc(int size);
extern void sgIP_free(void * ptr);
extern void sgIP_dbgprint(char * txt, ...);
extern void Timer_50ms(void);
extern void arm9_synctoarm7();
extern void arm9_fifo();
extern void PA_InitWifi();
extern bool PA_ConnectWifiWFC();
unsigned long PA_chartoip(char *message);
int PA_InitSocket(int *socket,char *host,int port,int mode);
int PA_InitServer(int *sock,int port,int mode, int num_connect);
int PA_GetHTTP(char *buffer, char *adress);
