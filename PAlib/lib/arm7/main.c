//////////////////////////////////////////////////////////////////////
// Simple ARM7 stub (sends RTC, TSC, and X/Y data to the ARM 9)
// -- joat
// -- modified by Darkain and others
//////////////////////////////////////////////////////////////////////

#include <PA7.h>
#include <command.h>
#include "microphone7.h"
/*
#include <NDS/NDS.h>
 
#include <NDS/ARM7/BIOS.h>
#include <NDS/ARM7/touch.h>
#include <NDS/ARM7/clock.h>
*/


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

	//SndVblIrq();	// DekuTree64's version :)	modified by JiaLing

}

/*
void InterruptHandler(void) {

 
  if (REG_IF & IRQ_VBLANK) {
	PA_VBL();
  }

  if (REG_IF & IRQ_TIMER0) {
	// DekuTree64's MOD player update
    SndTimerIrq();
  }

  if (REG_IF & IRQ_TIMER3) {
	PA_ProcessMicrophoneTimerIRQ();
    VBLANK_INTR_WAIT_FLAGS |= IRQ_TIMER3;
  }

  // Acknowledge interrupts
  REG_IF = REG_IF;
}*/
 

//////////////////////////////////////////////////////////////////////
 

int main(int argc, char ** argv) {
  
  PA_Init();

 /*
     TransferSound *snd = IPC->soundData;
    IPC->soundData = 0;
for (u8 i = 0; i < 16; i++) snd->data[i].vol = 0;*/	
  //enable sound
//  SOUND_CR = SCHANNEL_ENABLE | SOUND_VOL(0x7F);
//  IPC->soundData = 0;

	IPC->mailData=0;
	IPC->mailSize=0;

 	rtcReset();

	//enable sound
	powerON(POWER_SOUND);
	SOUND_CR = SOUND_ENABLE | SOUND_VOL(0x7F);
	IPC->soundData = 0;

	
	irqInit();
	irqSet(IRQ_VBLANK, PA_VBL);
	irqEnable(IRQ_VBLANK);
	irqSet(IRQ_TIMER0, SndTimerIrq);
	irqEnable(IRQ_TIMER0);	
	irqSet(IRQ_TIMER3, PA_ProcessMicrophoneTimerIRQ);
	irqEnable(IRQ_TIMER3);		
/*
  // Set up the interrupt handler
  REG_IME = 0;
  IRQ_HANDLER = &InterruptHandler;
  REG_IE = IRQ_VBLANK | IRQ_TIMER3|IRQ_TIMER0;
  REG_IF = ~0;
  DISP_SR = DISP_VBLANK_IRQ;
  REG_IME = 1;*/

  SndInit7 ();
    
  // Keep the ARM7 out of main RAM
  while (1) swiWaitForVBlank();
  return 0;
}

 
//////////////////////////////////////////////////////////////////////

