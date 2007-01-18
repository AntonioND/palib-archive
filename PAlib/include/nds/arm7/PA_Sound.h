#ifndef _PA_Sound7
#define _PA_Sound7

#ifdef __cplusplus
extern "C" {
#endif

#include <PA7.h>

#define MAX_SOUNDS 128 // Nombre maximum de sons à 32...
#define IPCSOUND 12  // Numéro de code pour le son en com...

#define STATE_NOTHING 0
#define STATE_PLAYING 1
#define STATE_STOPPED 2

#define ACTION_NOTHING 0
#define ACTION_PLAY 1
#define ACTION_STOP 2


extern PA_IPCType *PA_IPC;
extern u8 PA_SoundBusyInit;


void PA_SetDSLiteBrightness(u8 level);





extern inline void PA_Mic(void){
	PA_IPC->Mic.Volume = MIC_ReadData()-124; // Get volume
	if(PA_IPC->Mic.Data){ // Record new sound...
		StartRecording(PA_IPC->Mic.Data, PA_IPC->Mic.Length);
		PA_IPC->Mic.Data = 0;
	}
}


extern inline void PA_SoundUpdates(void){
u8 channel;
	if(PA_IPC->Sound[16].Volume) {  // Change global sound volume
		SOUND_CR = SOUND_ENABLE | SOUND_VOL(PA_IPC->Sound[16].Volume&127);
		PA_IPC->Sound[16].Volume = 0;
	}
	if(PA_IPC->Sound[16].Busy){  // Change Brightness
		PA_SetDSLiteBrightness(PA_IPC->Sound[16].Busy&3);
		PA_IPC->Sound[16].Busy = 0; // don't change anymore...
	}
	for (channel = 0; channel < 16; channel++) {
		PA_IPC->Sound[channel].Busy = SCHANNEL_CR(channel)>>31;
		
		if(PA_IPC->Sound[channel].Volume){ // If you need to change the sound volumes...
			SCHANNEL_CR(channel) &= ~SOUND_VOL(127); // reset sound volume
			SCHANNEL_CR(channel) |= SOUND_VOL(PA_IPC->Sound[channel].Volume&127);
			PA_IPC->Sound[channel].Volume = 0;
		}
		
		if(PA_IPC->Sound[channel].Pan){ // If you need to change the sound volumes...
			SCHANNEL_PAN(channel) = SOUND_VOL(PA_IPC->Sound[channel].Pan&127);
			PA_IPC->Sound[channel].Pan = 0;
		}		
		
	}	
}


extern inline void PA_SoundPlay(u8 channel) {
	SCHANNEL_TIMER(channel)  = SOUND_FREQ(PA_IPC->Sound[channel].Rate);
	SCHANNEL_SOURCE(channel) = (u32)PA_IPC->Sound[channel].Data;
	SCHANNEL_LENGTH(channel) = PA_IPC->Sound[channel].Length >> 2;
	SCHANNEL_CR(channel)     = SCHANNEL_ENABLE | SOUND_ONE_SHOT | SOUND_VOL(PA_IPC->Sound[channel].Volume) | SOUND_PAN(PA_IPC->Sound[channel].Pan) | ((PA_IPC->Sound[channel].Format)<<29);
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
		}
		PA_IPC->Sound[i].Command = 0;
	}

}


/*
extern inline void PA_SoundPlay(void)
{
	if (0 != snd) {

		for (i=0; i<snd->count; i++) {
			s32 chan = getFreeSoundChannel();

			if (chan >= 0) {
				PA_SoundPlayEx(snd->data[i].rate, snd->data[i].data, snd->data[i].len, chan, snd->data[i].vol, snd->data[i].pan, snd->data[i].format);
			}
		}
	}
}*/


//pause a song 0 for unpause 1 for pause
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


