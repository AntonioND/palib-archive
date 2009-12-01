#include "PA_Internal.h"
#include <string.h>

void PA_Init(bool sound){
	irqInit();
	fifoInit();
	readUserSettings();
	sleepIsEnabled = false; // disable libnds autosleep

	// Initialize the PAlib Fifo channel
	PA_InitFifo();

	// Initialize sound if requested
	if(sound) enableSound();

	// Some libnds initializations...
	initClockIRQ(); // init clock
	installSystemFIFO(); // needed by some libnds functions
	SetYtrigger(80); // needed by the touch screen code
	micOn(); // init microphone

	// The network IRQ is needed by the clock (yes)
	irqEnable(IRQ_NETWORK);
}
