#ifndef _PA_Sound7
#define _PA_Sound7

#ifdef __cplusplus
extern "C" {
#endif

#include <PA7.h>


extern volatile PA_IPCType *PA_IPC;
extern u8 PA_SoundBusyInit;
extern s32 modvolume; 

void PA_SetDSLiteBrightness(u8 level);

// Microphone variables
extern u8* microphone_buffer;
extern int microphone_buffer_length;
extern int current_length;


static inline void PA_Mic(void)
{
	// Get volume
	PA_IPC->Mic.Volume = micReadData8() - 124;
	
	// Record new sound...
	if (PA_IPC->Mic.Data)
	{
		// The following libnds function can't be used because it forces you to use TIMER0, which is needed by aslib:
		// StartRecording(PA_IPC->Mic.Data, PA_IPC->Mic.Length);
		// Copied code with TIMER0 -> TIMER2:
		microphone_buffer = PA_IPC->Mic.Data;
		microphone_buffer_length = PA_IPC->Mic.Length;
		current_length = 0;

		micOn();

		TIMER2_DATA = 0xF7CF;
		TIMER2_CR = TIMER_ENABLE | TIMER_DIV_1 | TIMER_IRQ_REQ;
		
		// Don't trigger this again
		PA_IPC->Mic.Data = 0;
	}
}

static inline void PA_SoundUpdates(void){
	u8 channel;
	static PA_IPCSound Soundold[17];
	
	if(PA_IPC->Sound[16].Volume!=Soundold[16].Volume) { // Change global sound volume
		SOUND_CR = SOUND_ENABLE | SOUND_VOL(PA_IPC->Sound[16].Volume&127);
		Soundold[16].Volume=PA_IPC->Sound[16].Volume;
	}
	if(PA_IPC->Sound[16].Busy){ // Change Brightness
		PA_SetDSLiteBrightness(PA_IPC->Sound[16].Busy&3);
		PA_IPC->Sound[16].Busy = 0; // don't change anymore...
	}
	for (channel = 0; channel < 16; channel++) {
		PA_IPC->Sound[channel].Busy = SCHANNEL_CR(channel)>>31;
		
		if(PA_IPC->Sound[channel].Volume!=Soundold[channel].Volume){ // If you need to change the sound volumes...
			SCHANNEL_CR(channel) &= ~SOUND_VOL(127); // reset sound volume
			SCHANNEL_CR(channel) |= SOUND_VOL(PA_IPC->Sound[channel].Volume&127);
			Soundold[channel].Volume=PA_IPC->Sound[channel].Volume;
		}
		
		if(PA_IPC->Sound[channel].Pan!=Soundold[channel].Pan){ // If you need to change the sound volumes...
			SCHANNEL_PAN(channel) = SOUND_VOL(PA_IPC->Sound[channel].Pan&127);
			Soundold[channel].Pan = PA_IPC->Sound[channel].Pan;
		}
	}
}

// below: old sound / mod functions (for some compatibility to PSG functions)

extern inline void PA_SoundPlay(u8 channel) {
	SCHANNEL_TIMER(channel)  = SOUND_FREQ(PA_IPC->Sound[channel].Rate);
	SCHANNEL_SOURCE(channel) = (u32)PA_IPC->Sound[channel].Data;
	SCHANNEL_LENGTH(channel) = PA_IPC->Sound[channel].Length >> 2;
	
	SCHANNEL_CR(channel)     = SCHANNEL_ENABLE | ((SOUND_ONE_SHOT)>>(PA_IPC->Sound[channel].Repeat)) | SOUND_VOL(PA_IPC->Sound[channel].Volume) | SOUND_PAN(PA_IPC->Sound[channel].Pan) | ((PA_IPC->Sound[channel].Format)<<29)|(PA_IPC->Sound[channel].Duty<<24);
	if(PA_IPC->Sound[channel].Repeat) SCHANNEL_REPEAT_POINT(channel) = PA_IPC->Sound[channel].RepeatPoint;
	else SCHANNEL_REPEAT_POINT(channel) = 0;
}

//stop a song
extern inline void PA_SoundStop(u8 channel)
{
	SCHANNEL_CR(channel) = 0; 
}

// Function to play/stop new sounds...
extern inline void PA_SoundProcess(void){
	u8 i;
	for (i = 0; i < 16; i++) // for all sounds
	{

		if(PA_IPC->Sound[i].Command) // Something to do...
		{
			if((PA_IPC->Sound[i].Command>>PAIPC_STOP)&1) PA_SoundStop(i);
			if((PA_IPC->Sound[i].Command>>PAIPC_PLAY)&1) PA_SoundPlay(i); // play sound
			PA_IPC->Sound[i].Command = 0;		
		}
		
	}

}

// pause a mod 0 for unpause 1 for pause
extern inline void PA_SoundPause(u8 channel)
{
	//Frequency = 0 => bits are read at 0hz => pause read
/*	if(cmd->param[1] == 1)
		SCHANNEL_TIMER(cmd->param[0]) = 0;
	else
		SCHANNEL_TIMER(cmd->param[0])  = SOUND_FREQ(sndChannel[cmd->param[0]].timer);*/
}

/** @} */ // end of SoundARM7

#ifdef __cplusplus
}
#endif

#endif
