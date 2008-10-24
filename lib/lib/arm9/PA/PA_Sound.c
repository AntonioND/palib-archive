#ifdef __cplusplus
extern "C" {
#endif

#include "PA_Sound.h"

void PA_PlayPSG(u8 PA_Channel, u8 vol, u8 pan, s32 freq, u8 duty){  
	PA_IPC.Sound[PA_Channel].Busy = 1; // now busy
	PA_IPC.Sound[PA_Channel].Command |= (1<<PAIPC_PLAY); // play
	PA_IPC.Sound[PA_Channel].Volume = vol;  
	PA_IPC.Sound[PA_Channel].Pan = pan; 
	PA_IPC.Sound[PA_Channel].Rate = freq;
	PA_IPC.Sound[PA_Channel].Format = 3;// PSG Sound	
	PA_IPC.Sound[PA_Channel].Repeat = 0;
	PA_IPC.Sound[PA_Channel].Duty = duty&7; // Limit values
}

void PA_PlaySoundEx2(u8 PA_Channel, const void* data, s32 length, u8 volume, int freq, s16 format, bool repeat, int repeatPoint){

    DC_FlushAll();
	PA_IPC.Sound[PA_Channel].Busy = 1; // now busy
	
	// keep this in for compatibility with some functions
	// Sound commands
	//PA_IPC.Sound[PA_Channel].Command |= (1<<PAIPC_PLAY); // play
	PA_IPC.Sound[PA_Channel].Data = (u32*)data;
	PA_IPC.Sound[PA_Channel].Volume = volume;  
	PA_IPC.Sound[PA_Channel].Pan = 64; 
	PA_IPC.Sound[PA_Channel].Rate = freq;
	PA_IPC.Sound[PA_Channel].Length = length-4;
	PA_IPC.Sound[PA_Channel].Format = format;	
	PA_IPC.Sound[PA_Channel].Repeat = repeat;
	PA_IPC.Sound[PA_Channel].RepeatPoint = repeatPoint;
	PA_IPC.Sound[PA_Channel].Duty = 0;
//	PA_IPC.Sound[PA_Channel].Pause = 0;
	PA_IPC.Sound[PA_Channel].Command |= (1<<PAIPC_PLAY); // play

}

void PA_StopSound(u8 PA_Channel){
	PA_IPC.Sound[PA_Channel].Command |= (1<<PAIPC_STOP);
}

#ifdef __cplusplus
}
#endif
