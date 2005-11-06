//////////////////////////////////////////////////////////////////////
// Simple ARM7 stub (sends RTC, TSC, and X/Y data to the ARM 9)
// -- joat
// -- modified by Darkain and others
//////////////////////////////////////////////////////////////////////

#include <PA7.h>
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
  SCHANNEL_CR(channel) = SCHANNEL_ENABLE | SOUND_ONE_SHOT | SOUND_VOL(vol) | SOUND_PAN(pan) | ((format == 1) ? SOUND_8BIT : SOUND_16BIT);
}


s8 getFreeSoundChannel() {
int i;
  for (i=0; i<16; i++) {
    if ( (SCHANNEL_CR(i) & SOUND_ENABLE) == 0 ) return i;
  }
  return -1;
}


//////////////////////////////////////////////////////////////////////


void InterruptHandler(void) {
  static int heartbeat = 0;
 
  if (REG_IF & IRQ_VBLANK) {
    s32 but=0, batt=0, aux=0;
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
    aux  = touchRead(TSC_MEASURE_AUX);

    // Read the time
    rtcGetTime((uint8 *)ct);
    BCDToInteger((uint8 *)&(ct[1]), 7);
 
    // Read the temperature
    temp = touchReadTemperature(&t1, &t2);
 
    // Update the IPC struct
    IPC->heartbeat = heartbeat;
    IPC->buttons   = but;
//    IPC->touchX    = x;
//   IPC->touchY    = y;
//    IPC->touchXpx  = xpx;
//    IPC->touchYpx  = ypx;
//    IPC->touchZ1   = z1;
//    IPC->touchZ2   = z2;
    IPC->battery   = batt;
    IPC->aux       = aux;
    u32 i;
    for(i=0; i<sizeof(ct); i++) {
      IPC->curtime[i] = ct[i];
    }

    IPC->temperature = temp;
    IPC->tdiode1 = t1;
    IPC->tdiode2 = t2;
	/*
	if (screenlights != (IPC->aux&0xC)){
		screenlights = IPC->aux&0xC;
		PA_ScreenLight(); // Update the screen lights...
	}*/

/*
    //sound code   :) 
    TransferSound *snd = IPC->soundData;
    IPC->soundData = 0;
    if (snd) {
      for (int i=0; i<snd->count; i++) {
        s8 chan = getFreeSoundChannel();
        if (chan >= 0) {
          startSound(snd->data[i].rate, snd->data[i].data, snd->data[i].len, chan, snd->data[i].vol, snd->data[i].pan, snd->data[i].format);
        }
      }
    }*/

	//sound code   :) 
    TransferSound *snd = IPC->soundData;
    IPC->soundData = 0;
	//u8 currentchan = 15;
    if (snd) {
    s8 i;
      for (i=0; i<8; i++) {
 //       s8 chan = getFreeSoundChannel();
        if (snd->data[i].vol > 128) {  // Si volume, indique qu'il y a un son...
			//if ( (SCHANNEL_CR(i) & SOUND_ENABLE) == 0 )
			snd->data[i].vol -= 128;
			startSound(snd->data[i].rate, snd->data[i].data, snd->data[i].len, i+8,snd->data[i].vol, snd->data[i].pan, snd->data[i].format);
		}
      }
    }
	SndVblIrq();	// DekuTree64's version :)	

  }

  if (REG_IF & IRQ_TIMER0) {
		// DekuTree64's MOD player update
    SndTimerIrq();
  }

  // Acknowledge interrupts
  REG_IF = REG_IF;
}
 

//////////////////////////////////////////////////////////////////////
 

int main(int argc, char ** argv) {
  // Reset the clock if needed
  rtcReset();
  
  PA_Init();

 /*
     TransferSound *snd = IPC->soundData;
    IPC->soundData = 0;
for (u8 i = 0; i < 16; i++) snd->data[i].vol = 0;*/	
  //enable sound
//  SOUND_CR = SCHANNEL_ENABLE | SOUND_VOL(0x7F);
//  IPC->soundData = 0;
 


  // Set up the interrupt handler
  REG_IME = 0;
  IRQ_HANDLER = &InterruptHandler;
  REG_IE = IRQ_VBLANK;
  REG_IF = ~0;
  DISP_SR = DISP_VBLANK_IRQ;
  REG_IME = 1;

  SndInit7 ();
  
  // Keep the ARM7 out of main RAM
  while (1) swiWaitForVBlank();
  return 0;
}

 
//////////////////////////////////////////////////////////////////////

