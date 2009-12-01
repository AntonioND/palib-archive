#include "PA_Internal.h"

vu8* PA_MicBuffer = 0;
vu32 PA_MicBufferLen = 0;

void PA_FifoMicMsg(PA_FifoMsg* msg){
	PA_MicBuffer = msg->MicMsg.buffer;
	PA_MicBufferLen = msg->MicMsg.length;

	// Start recording (16384 Hz)
	// You gotta love the new libnds...
	timerStart(2, ClockDivider_1, TIMER_FREQ(16384), PA_MicTimer);
}

void PA_MicTimer(){
	if(PA_MicBuffer && (PA_MicBufferLen > 0)){
		// Read data from the microphone. Data from the Mic is unsigned, flipping
		// the highest bit makes it signed.
		*PA_MicBuffer++ = micReadData8() ^ 0x80;
		PA_MicBufferLen--;
	}else // as we have finished the recording
		timerStopEx(2); // stop the recording timer
}
