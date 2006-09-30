//////////////////////////////////////////////////////////////////////
// Simple ARM7 stub (sends RTC, TSC, and X/Y data to the ARM 9)
// -- joat
// -- modified by Darkain and others
//////////////////////////////////////////////////////////////////////

#include <PA7.h>
#include <command.h>
#include "microphone7.h"
#include <dswifi7.h>
/*
#include <NDS/NDS.h>
 
#include <NDS/ARM7/BIOS.h>
#include <NDS/ARM7/touch.h>
#include <NDS/ARM7/clock.h>
*/

PA_IPCType *PA_IPC;
u8 PA_SoundBusyInit;


//////////////////////////////////////////////////////////////////////

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


//////////////////////////////////////////////////////////////////////
//u8 testvar = 0;

void PA_VBL(void){
  static int heartbeat = 0;
   s32 but=0, batt=0;// aux=0;
    int t1=0, t2=0;
    uint32 temp=0;
    uint8 ct[sizeof(IPC->curtime)];

    // Update the heartbeat
    heartbeat++;
 
    // Read the X/Y buttons and the /PENIRQ line
    but = REG_KEYXY;
    if (!(but & 0x40)) {
		PA_UpdateStylus();
		/*MIC_On();
		StartRecording((u8*)33808000, 100000);
		*(u8*)33807928 = 1;*/
    }

    batt = touchRead(TSC_MEASURE_BATTERY);
    
    // Read the time
    rtcGetTime((uint8 *)ct);
    BCDToInteger((uint8 *)&(ct[1]), 7);
 
    // Read the temperature
    temp = touchReadTemperature(&t1, &t2);
    IPC->heartbeat = heartbeat;
    IPC->buttons   = but;
    IPC->battery   = batt;
    
    u32 i;
    for(i=0; i<sizeof(ct); i++) {
      IPC->curtime[i] = ct[i];
    }

    IPC->temperature = temp;
    IPC->tdiode1 = t1;
    IPC->tdiode2 = t2;
	
	if (PA_NewSPI != (IPC->aux)){
		PA_NewSPI = IPC->aux;
		PA_ScreenLight(); // Update the screen lights...
		//IPC->aux = touchRead(TSC_MEASURE_AUX); // update IPC with new values
	}
	
	u8 channel;
			
	SndVblIrq();	// DekuTree64's version :)	modified by JiaLing
	
	if(PA_SoundBusyInit){  // Sound ready to use...
		if(PA_IPC[0].MasterVol) {  // Change global sound volume
			SOUND_CR = SOUND_ENABLE | SOUND_VOL(PA_IPC[0].MasterVol);
			PA_IPC[0].MasterVol = 0;
		}
		for (channel = 0; channel < 16; channel++) {
			PA_IPC[channel].Busy = SCHANNEL_CR(channel)>>31;
			if(PA_IPC[channel].ChangeVolume){ // If you need to change the sound volumes...
				PA_IPC[channel].ChangeVolume = 0;
				SCHANNEL_CR(channel) &= ~SOUND_VOL(127); // reset sound volume
				SCHANNEL_CR(channel) |= SOUND_VOL(PA_IPC[channel].Volume);
			}
		}	
	}
	else if(IPC->mailData != 0) {
		PA_IPC = (PA_IPCType*)(IPC->mailData); // Inits PA Sound busy commands
		IPC->mailData = 0;
		PA_SoundBusyInit = 1;
	}
	
Wifi_Update();
}


 

//////////////////////////////////////////////////////////////////////
 
 
void timer0(void){

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

  


 /*
     TransferSound *snd = IPC->soundData;
    IPC->soundData = 0;
for (u8 i = 0; i < 16; i++) snd->data[i].vol = 0;*/	
  //enable sound
//  SOUND_CR = SCHANNEL_ENABLE | SOUND_VOL(0x7F);
//  IPC->soundData = 0;



	PA_Init();
	irqInit();
	irqSet(IRQ_VBLANK, PA_VBL);
	irqEnable(IRQ_VBLANK);
	irqSet(IRQ_TIMER0, timer0);
	irqEnable(IRQ_TIMER0);	
	irqSet(IRQ_TIMER3, ProcessMicrophoneTimerIRQ);
	irqEnable(IRQ_TIMER3);	
	
	//supprime pour test
    irqSet(IRQ_WIFI, Wifi_Interrupt); // set up wifi interrupt
	irqEnable(IRQ_WIFI);

/*
  // Set up the interrupt handler
  REG_IME = 0;
  IRQ_HANDLER = &InterruptHandler;
  REG_IE = IRQ_VBLANK | IRQ_TIMER3|IRQ_TIMER0;
  REG_IF = ~0;
  DISP_SR = DISP_VBLANK_IRQ;
  REG_IME = 1;*/
  
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

