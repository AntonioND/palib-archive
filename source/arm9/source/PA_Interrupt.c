#define PA_NO_DEPRECATION
#include <PA9.h>

//////////////////////////////////////////////////////////////////////
// Interrupts
//////////////////////////////////////////////////////////////////////

funcpointer CustomVBL = PA_Nothing;
funcpointer MotionVBL = PA_Nothing;
funcpointer GHPadVBL  = PA_Nothing;
funcpointer PaddleVBL = PA_Nothing;
funcpointer SpriteVBL = PA_Nothing;

s32 PA_VBLCounter[16]; // VBL counters
bool PA_VBLCounterOn[16]; // VBL counters enable/disable flag

//static bool PA_RandInit = false;

void PA_VBLCountersReset(){
	register int i;
	for (i = 0; i < 16; i++) {
		PA_VBLCounter[i] = 0; // Ready to start
		PA_VBLCounterOn[i] = 0;  // Not counting yet
	}
}

static inline void PA_RunCounters(){
	register int i;
	for(i = 0; i < 16; i++)
		PA_VBLCounter[i] += PA_VBLCounterOn[i];
}

void PA_vblFunc(){
	// Flush the legacy IPC
	DC_FlushRange((void*) &PA_IPC, sizeof(PA_IPCType));

	// Update the Pad and the Stylus
	PA_UpdatePad();
	PA_UpdateStylus();

	SpriteVBL();

	// Update the external pads
	MotionVBL();
	GHPadVBL();
	PaddleVBL();

	// Update the RTC
	PA_UpdateRTC();

	// Run the counters
	PA_RunCounters();

	// Run the user's custom VBL function
	CustomVBL();
}
