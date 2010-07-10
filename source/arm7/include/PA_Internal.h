#pragma once

#include <PA7.h>

// Sandbox START - should be organized ///////////////////////////

// Um... This should be in libnds
#define PM_DSLITE_REG 4

extern bool sleepIsEnabled; // from libnds

void enableSound(); // a libnds function that isn't in the headers.

// Improved version of timerStop()
static inline void timerStopEx(int channel){
	TIMER_CR(channel) = 0;
	TIMER_DATA(channel) = 0;
	irqDisable(IRQ_TIMER(channel));
	irqSet(IRQ_TIMER(channel), 0);
}

// Sandbox END ///////////////////////////////////////////////////

// Fifo functions
void PA_InitFifo();
void PA_FifoMsgHandler(int bytes, void* dummy);
void PA_FifoValHandler(u32 type, void* dummy);

// Microphone functions
void PA_MicTimer();
void PA_FifoMicMsg(PA_FifoMsg* msg);
static inline void PA_MicStop(){ timerStopEx(2); }

// PSG functions
void PA_FifoPSGMsg(PA_FifoMsg* msg);
