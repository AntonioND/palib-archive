//////////////////////////////////////////////////////////////////////
// Simple ARM7 stub (sends RTC, TSC, and X/Y data to the ARM 9)
// -- joat
// -- modified by Darkain and others
//////////////////////////////////////////////////////////////////////

#include <PA7.h>
#include <nds.h>
#include <stdlib.h>

#include "as_lib7.h"

// <LIBLOBBY>
#include "smi_startup_viaipc.h"
#include "MessageQueue.h"
#include "wifi_hal.h"

int vcount;
touchPosition first, tempPos;

u8* microphone_buffer = 0;
int microphone_buffer_length = 0;
int current_length = 0;

void PA_IPCManage(void){
	if(PA_SoundBusyInit) {  // Sound ready to use...
		PA_Mic(); // Manage Mic
		PA_SoundProcess(); // Play/stop cmomands
		PA_SoundUpdates();  // Get busy sound channels, change volume...
	}
	else if(IPC->mailData != 0) {
		PA_IPC = (PA_IPCType*)(IPC->mailData); // Inits PA Sound busy commands
		IPC->mailData = 0;
		PA_SoundBusyInit = 1;
	}

}

void VcountHandler() {
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

#define READ_DATA_REG1     0x65
#define READ_STATUS_REG1   0x61

void PA_rtcGetTime(uint8 * time) {
	uint8 command, status;

	time[0] = READ_DATA_REG1;
	rtcTransaction(&(time[0]), 1, &(time[1]), 7);

	command = READ_STATUS_REG1;
	rtcTransaction(&command, 1, &status, 1);
	time[0] = status;
}

void PA_VBL() {
   s32 batt=0;
    int t1=0, t2=0;
    uint32 temp=0;
    uint8 ct[sizeof(IPC->time.curtime)];

    batt = touchRead(TSC_MEASURE_BATTERY);
    
    // Read the time
    PA_rtcGetTime((uint8 *)ct);
    BCDToInteger((uint8 *)&(ct[1]), 7);
 
    // Read the temperature
    temp = touchReadTemperature(&t1, &t2);
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
	}
	
	AS_SoundVBL();	// Update AS_Lib
	
	PA_IPCManage(); // PAlib IPC functions (sound busy, panning, etc...)
}

void timer2(void){
	// See comment in PA_Mic().
	// ProcessMicrophoneTimerIRQ();
	
	if(microphone_buffer && microphone_buffer_length > 0) {
	  // Read data from the microphone. Data from the Mic is unsigned, flipping
	  // the highest bit makes it signed.
	  *microphone_buffer++ = MIC_ReadData() ^ 0x80;
	  --microphone_buffer_length;
	  current_length++;
	}
}

int main() {
	IPC->mailData=0;
	IPC->mailSize=0; 
	PA_SoundBusyInit = 0;

  
	PA_Init();
	irqInit();
	irqSet(IRQ_VBLANK, PA_VBL);
	irqEnable(IRQ_VBLANK);
	irqSet(IRQ_TIMER2, timer2);
	irqEnable(IRQ_TIMER2);
	
	SetYtrigger(80);
	vcount = 80;
	irqSet(IRQ_VCOUNT, VcountHandler);
	irqEnable(IRQ_VBLANK | IRQ_VCOUNT);
	
	//supprime pour test
	SndInit7 ();
	
	// Initialize AS_Lib
	AS_Init();
	
	// <LIBLOBBY>
	IPC_Init();
	LWIFI_Init();

	while (1) {
		AS_MP3Engine();
		LWIFI_IPC_UpdateNOIRQ();	// <LIBLOBBY>
	}
	
	return 0;
}
