//////////////////////////////////////////////////////////////////////
// Simple ARM7 stub (sends RTC, TSC, and X/Y data to the ARM 9)
// -- joat
// -- modified by Darkain and others
//////////////////////////////////////////////////////////////////////

#include <PA7.h>
//#include <command.h>
//#include "microphone7.h"
#include <dswifi7.h>
#include <nds.h>
#include <stdlib.h>
/*
#include <NDS/NDS.h>
 
#include <NDS/ARM7/BIOS.h>
#include <NDS/ARM7/touch.h>
#include <NDS/ARM7/clock.h>
*/



//////////////////////////////////////////////////////////////////////
/*
void startSound(int sampleRate, const void* data, uint32 bytes, u8 channel, u8 vol,  u8 pan, u8 format) {
  SCHANNEL_TIMER(channel)  = SOUND_FREQ(sampleRate);
  SCHANNEL_SOURCE(channel) = (uint32)data;
  SCHANNEL_LENGTH(channel) = bytes >> 2;
  SCHANNEL_REPEAT_POINT(channel) = 0;
  SCHANNEL_CR(channel) = SCHANNEL_ENABLE | SOUND_ONE_SHOT | SOUND_VOL(vol) | SOUND_PAN(pan) | format;
}

s8 getFreeSoundChannel() {
int i;
  for (i=0; i<16; i++) {
    if ( (SCHANNEL_CR(i) & SOUND_ENABLE) == 0 ) return i;
  }
  return -1;
}
*/



void PA_IPCManage(void){
	if(PA_SoundBusyInit){  // Sound ready to use...
		PA_Mic(); // Manage Mic
		PA_SoundProcess(); // Play/stop cmomands
		PA_SoundUpdates();  // Get busy sound channels, change volume...
	//	PA_UpdatePad();
	}
	else if(IPC->mailData != 0) {
		PA_IPC = (PA_IPCType*)(IPC->mailData); // Inits PA Sound busy commands
		IPC->mailData = 0;
		PA_SoundBusyInit = 1;
	}

}


int vcount;
touchPosition first,tempPos;


//---------------------------------------------------------------------------------
void VcountHandler() {
//---------------------------------------------------------------------------------
	static int lastbut = -1;
	
	uint16 but=0, x=0, y=0, xpx=0, ypx=0, z1=0, z2=0;

	but = REG_KEYXY;

	if (!( (but ^ lastbut) & (1<<6))) {
 
		tempPos = touchReadXY();

		if ( tempPos.x == 0 || tempPos.y == 0 ) {
			but |= (1 <<6);
			lastbut = but;
		} else {
			x = tempPos.x;
			y = tempPos.y;
			xpx = tempPos.px;
			ypx = tempPos.py;
			z1 = tempPos.z1;
			z2 = tempPos.z2;
		}
		
	} else {
		lastbut = but;
		but |= (1 <<6);
	}

	if ( vcount == 80 ) {
		first = tempPos;
	} else {
		if (	abs( xpx - first.px) > 10 || abs( ypx - first.py) > 10 ||
				(but & ( 1<<6)) ) {

			but |= (1 <<6);
			lastbut = but;

		} else { 	
			IPC->mailBusy = 1;
			IPC->touchX			= x;
			IPC->touchY			= y;
			IPC->touchXpx		= xpx;
			IPC->touchYpx		= ypx;
			IPC->touchZ1		= z1;
			IPC->touchZ2		= z2;
			IPC->mailBusy = 0;
		}
	}
	IPC->buttons		= but;
	vcount ^= (80 ^ 130);
	SetYtrigger(vcount);

}



//////////////////////////////////////////////////////////////////////
//u8 testvar = 0;

void PA_VBL(void){
//  static int heartbeat = 0;
   s32 batt=0;// aux=0;
    int t1=0, t2=0;
    uint32 temp=0;
    uint8 ct[sizeof(IPC->time.curtime)];

    // Update the heartbeat
 //   heartbeat++;
 
    // Read the X/Y buttons and the /PENIRQ line
 /*   but = REG_KEYXY;
    if (!(but & 0x40)) {   //
		PA_UpdateStylus();  // If IPC set correctly
    }*/

    batt = touchRead(TSC_MEASURE_BATTERY);
    
    // Read the time
    rtcGetTime((uint8 *)ct);
    BCDToInteger((uint8 *)&(ct[1]), 7);
 
    // Read the temperature
    temp = touchReadTemperature(&t1, &t2);
//    IPC->heartbeat = heartbeat;
  //  IPC->buttons   = but;
    IPC->battery   = batt;
    
    u32 i;
    for(i=0; i<sizeof(ct); i++) {
      IPC->time.curtime[i] = ct[i];
    }

    IPC->temperature = temp;
    IPC->tdiode1 = t1;
    IPC->tdiode2 = t2;
	
	if (PA_NewSPI != (IPC->aux)){
		PA_NewSPI = IPC->aux;
		PA_ScreenLight(); // Update the screen lights...
		//IPC->aux = touchRead(TSC_MEASURE_AUX); // update IPC with new values
	}
			
	SndVblIrq();	// DekuTree64's version
	
	
	PA_IPCManage(); // PAlib IPC functions (sound busy, panning, etc...)
	
	Wifi_Update();
}


 

//////////////////////////////////////////////////////////////////////
 
 
void timer0(void){
	ProcessMicrophoneTimerIRQ();
	SndTimerIrq();
}
 
void arm7_synctoarm9() { // send fifo message
   REG_IPC_FIFO_TX = 0x87654321;
}
// interrupt handler to allow incoming notifications from arm9
void arm7_fifo() { // check incoming fifo messages
   u32 msg = REG_IPC_FIFO_RX;
   if(msg==0x87654321) Wifi_Sync();
}

int main(int argc, char ** argv) {
  REG_IPC_FIFO_CR = IPC_FIFO_ENABLE | IPC_FIFO_SEND_CLEAR;
IPC->mailData=0;
IPC->mailSize=0; 
  PA_SoundBusyInit = 0;

  
	PA_Init();
	irqInit();
	irqSet(IRQ_VBLANK, PA_VBL);
	irqEnable(IRQ_VBLANK);
	irqSet(IRQ_TIMER0, timer0);
	irqEnable(IRQ_TIMER0);	
	
	SetYtrigger(80);
	vcount = 80;
	irqSet(IRQ_VCOUNT, VcountHandler);
	irqEnable(IRQ_VBLANK | IRQ_VCOUNT);
//	irqSet(IRQ_TIMER3, ProcessMicrophoneTimerIRQ);
//	irqEnable(IRQ_TIMER3);	

	//swiWaitForVBlank();
	//swiWaitForVBlank();
	
	//supprime pour test
    irqSet(IRQ_WIFI, Wifi_Interrupt); // set up wifi interrupt
	irqEnable(IRQ_WIFI);
  
	//swiWaitForVBlank();
	//swiWaitForVBlank();
  
 //supprime pour test
  SndInit7 ();
    
  u32 fifo_temp;   

	  while(1) { // wait for magic number
    	while(REG_IPC_FIFO_CR&IPC_FIFO_RECV_EMPTY) swiWaitForVBlank();
      fifo_temp=REG_IPC_FIFO_RX;
      if(fifo_temp==0x12345678) break;
   	}
   	while(REG_IPC_FIFO_CR&IPC_FIFO_RECV_EMPTY) swiWaitForVBlank();
   	fifo_temp=REG_IPC_FIFO_RX; // give next value to wifi_init
   	Wifi_Init(fifo_temp);
   	
   	irqSet(IRQ_FIFO_NOT_EMPTY,arm7_fifo); // set up fifo irq
   	irqEnable(IRQ_FIFO_NOT_EMPTY);
   	REG_IPC_FIFO_CR = IPC_FIFO_ENABLE | IPC_FIFO_RECV_IRQ;

   	Wifi_SetSyncHandler(arm7_synctoarm9); // allow wifi lib to notify arm9
  
  // Keep the ARM7 out of main RAM

  while (1) {
	swiWaitForVBlank();
	}
  return 0;
}

 
//////////////////////////////////////////////////////////////////////

