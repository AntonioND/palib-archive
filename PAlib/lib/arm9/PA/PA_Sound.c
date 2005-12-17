#include "PA9.h"

PA_SoundOptions PA_SoundOption; // Default sound formats...
u8 *FS_mod;
u8 *FS_wav[16];

void PA_SetDefaultSound(u8 volume, s16 freq, s16 format){
	PA_SoundOption.volume = volume;
	PA_SoundOption.freq = freq;
	PA_SoundOption.format = format;
}

		 
void PA_PlaySoundEx(u8 PA_Channel, const void* data, u32 length, u8 volume, s16 freq, s16 format){
//		PA_Channel = 15 - PA_Channel;
        snd.data[PA_Channel].data = data;
        snd.data[PA_Channel].len = length;
        snd.data[PA_Channel].rate = freq;
        snd.data[PA_Channel].pan = 64;
        snd.data[PA_Channel].vol = volume + 128; // Dit que c activé
        snd.data[PA_Channel].format = format;
        DC_FlushAll();
        IPC->soundData = &snd;
}
