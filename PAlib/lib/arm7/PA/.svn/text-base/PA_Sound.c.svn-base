
#include "PA7.h"


PA_SoundInfo PA_Sound[MAX_SOUNDS];  // Nombre maximum de sons à 32...

StartSoundInfo* PA_StartSoundInfo; // Permet de démarrer ou arreter le son

u8 PA_SoundOk = 0;

void PA_InitSound(s32 Sound_Num, s32 Rate, u8* Data, u32 Length){
	PA_Sound[Sound_Num].Rate = Rate;
	PA_Sound[Sound_Num].Raw = Data;
	PA_Sound[Sound_Num].Length = Length;
}



void PA_StartSound(u8 Channel, s32 Sound_Num, u8 Volume){

	SCHANNEL_TIMER(Channel) = SOUND_FREQ(PA_Sound[Sound_Num].Rate);
	SCHANNEL_SOURCE(Channel) = (u32)PA_Sound[Sound_Num].Raw;
	SCHANNEL_LENGTH(Channel) = PA_Sound[Sound_Num].Length;
	SCHANNEL_CR(Channel) = SOUND_ENABLE | SOUND_VOL(Volume&127);

	PA_StartSoundInfo[Channel].State = STATE_PLAYING; // Lecture en cours...
	PA_StartSoundInfo[Channel].Action = 0;
}




