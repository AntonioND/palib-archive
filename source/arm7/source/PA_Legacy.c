#include "PA_Internal.h"

void PA_LegacySoundUpdates();
void runNdsLoaderCheck();

bool PA_LegacyInit = false;

volatile PA_IPCType* PA_IPC;

void PA_LegacyIPCManage(){
	// For the NDS loader, not really a legacy function though
	runNdsLoaderCheck();

	if(PA_LegacyInit){
		// Legacy sound system
		PA_LegacySoundUpdates();
	}else if(PA_Transfer->mailData != 0){
		// Get the legacy IPC structure
		PA_IPC = (PA_IPCType*)(PA_Transfer->mailData);
		PA_Transfer->mailData = 0;
		PA_LegacyInit = true;
	}
}

/////////////////////////
// Legacy sound system //
/////////////////////////

void PA_SoundPlay(int channel);
void PA_SoundStop(int channel);

void PA_LegacySoundUpdates(){
	int channel;
	static PA_IPCSound Soundold[17];
	
	// Update global sound volume
	if(PA_IPC->Sound[16].Volume != Soundold[16].Volume){
		REG_SOUNDCNT = SOUND_ENABLE | SOUND_VOL(PA_IPC->Sound[16].Volume & 127);
		Soundold[16].Volume = PA_IPC->Sound[16].Volume;
	}

	for(channel = 0; channel < 16; channel++){
		if(PA_IPC->Sound[channel].Command){ // Play/stop sounds
			if((PA_IPC->Sound[channel].Command >> PAIPC_STOP) & 1) PA_SoundStop(channel);
			if((PA_IPC->Sound[channel].Command >> PAIPC_PLAY) & 1) PA_SoundPlay(channel);
			PA_IPC->Sound[channel].Command = 0;
		}

		// Update busy flags
		PA_IPC->Sound[channel].Busy = SCHANNEL_CR(channel)>>31;
		
		// Update channel volume
		if(PA_IPC->Sound[channel].Volume != Soundold[channel].Volume){
			SCHANNEL_CR(channel) &= ~SOUND_VOL(127); // reset sound volume
			SCHANNEL_CR(channel) |= SOUND_VOL(PA_IPC->Sound[channel].Volume & 127);
			Soundold[channel].Volume = PA_IPC->Sound[channel].Volume;
		}
		
		// Update channel pan
		if(PA_IPC->Sound[channel].Pan != Soundold[channel].Pan){
			SCHANNEL_PAN(channel) = SOUND_VOL(PA_IPC->Sound[channel].Pan & 127);
			Soundold[channel].Pan = PA_IPC->Sound[channel].Pan;
		}
	}
}

void PA_SoundPlay(int channel){
	SCHANNEL_TIMER(channel)  = SOUND_FREQ(PA_IPC->Sound[channel].Rate);
	SCHANNEL_SOURCE(channel) = (u32) PA_IPC->Sound[channel].Data;
	SCHANNEL_LENGTH(channel) = PA_IPC->Sound[channel].Length >> 2;

	SCHANNEL_CR(channel) = SCHANNEL_ENABLE |
		(SOUND_ONE_SHOT >> PA_IPC->Sound[channel].Repeat) |
		SOUND_VOL(PA_IPC->Sound[channel].Volume) |
		SOUND_PAN(PA_IPC->Sound[channel].Pan) |
		(PA_IPC->Sound[channel].Format << 29) |
		(PA_IPC->Sound[channel].Duty << 24);

	if(PA_IPC->Sound[channel].Repeat)
		SCHANNEL_REPEAT_POINT(channel) = PA_IPC->Sound[channel].RepeatPoint;
	else
		SCHANNEL_REPEAT_POINT(channel) = 0;
}

void PA_SoundStop(int channel){
	SCHANNEL_CR(channel) = 0; 
}

///////////////////////////////////////////////////////////
// This is not a legacy function but useful nevertheless //
///////////////////////////////////////////////////////////

void runNdsLoaderCheck(){
	if(*((vu32*)0x02FFFE24) == (u32)0x02FFFE04){
		irqDisable(IRQ_ALL);
		*((vu32*)0x02FFFE34) = (u32)0x06000000;
		swiSoftReset();
	}
}
