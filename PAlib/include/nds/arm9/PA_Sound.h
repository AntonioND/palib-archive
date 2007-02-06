#ifndef _PA_Sound9
#define _PA_Sound9



/*! \file PA_Sound.h
    \brief Sound info

    Play sounds ! Arm9 only
*/


#include "../Sound9.h"
#include "PA_GBFS.h"
#include "PA_FS.h"
#include <PA9.h>

	// 256KB to load MOD files into. Actual size necessary is the size 
	// of your largest MOD file (probably less than 256KB).
#define SND_MEM_POOL_SIZE	256*1024


extern TransferSound snd;
extern u32 *sndMemPool;

typedef struct{ // Default sound format
	u8 volume;
	s16 freq;
	s16 format;
} PA_SoundOptions;

extern PA_SoundOptions PA_SoundOption;

extern u32 *FS_mod;
extern u32 *FS_wav[16]; // 16 channels...



//Stream vars
extern u32 *Stream_Gap[16];
extern u32 *Stream_End[16];
extern u32 *Stream_Datas[16]; 
extern u32 *Stream_Regen_Ptr[16];
extern s64	Stream_Length[16];
extern u32	Stream_Last_Tick[16];
extern s32	Stream_Repeat[16];
extern u8	Stream_Timer[16];

//plays an 8 bit mono sample at 11025Hz






/** @defgroup SoundARM9 Sound ARM9 functions
 *  Play sounds, etc...
 *  @{
 */


/*! \fn void PA_SetDefaultSound(u8 volume, int freq, s16 format)
    \brief
         \~english Set the default sound options (for the PlaySimpleSound function)
         \~french Permet de régler options par défaut pour la fonction PlaySimpleSound
    \param volume
         \~english Volume, from 0 to 127. 64 if not used
         \~french Volume, de 0 à 127. 64 si rien n'est mis
    \param freq
         \~english Sound frequence, depends on the sound... 11025 by default
         \~french Fréquence du son...11025 par défaut
    \param format
         \~english Sound format.
         \~french Format du son.
*/
void PA_SetDefaultSound(u8 volume, int freq, s16 format);


/*! \fn extern inline void PA_InitSound(void)
    \brief
         \~english Initialise the Sound system, for mods and sound effects
         \~french Initialiser les sons, pour le mod player et les effets sonores
*/
extern inline void PA_InitSound(void) {
	SndInit9 ();
	sndMemPool = (u32*)0x2200000;
	SndSetMemPool(sndMemPool, SND_MEM_POOL_SIZE);
	PA_SetDefaultSound(127, 11025, 0);
	FS_mod = NULL; // Initialise a small portion of memory, will make it bigger later on...
	u8 i;
	for (i = 0; i < 16; i++)
	{
		FS_wav[i] = NULL; // to be able to free
	}

}



/*! \fn extern void PA_InitSound(void)
    \brief
         \~english Initialise the Stream system
         \~french Initialiser le système de stream
*/
extern void PA_InitStreamSound(void);

/*! \fn extern void PA_RegenStream(void)
    \brief
         \~english Test if a stream need regen
         \~french Teste si un stream doit etre régénéré
*/
extern void PA_RegenStream(void);

/*! \fn extern void FillTheGap(u8 PA_Channel, u32 size);
    \brief
         \~english Fill datas in a stream tab
         \~french Remplit de données un tableau de stream
*/
extern void FillTheGap(u8 PA_Channel, u32 size);

/*! \fn void PA_PlaySoundEx2(u8 PA_Channel, const void* data, s32 length, u8 volume, int freq, s16 format, BOOL repeat, int repeatPoint)
    \brief
         \~english Play a given sound effect, but chose your format
         \~french Joue une fois un son, mais en choisissant le format
    \param PA_Channel
         \~english Audio channel, from 0 to 7
         \~french Canal audio, de 0 à 7
    \param data
         \~english Sound data
         \~french Données du son
    \param length
         \~english Sound length, with (u32)sound_size
         \~french Longueur du son, avec (u32)sound_size
    \param volume
         \~english Volume, from 0 to 127. 64 if not used
         \~french Volume, de 0 à 127. 64 si rien n'est mis
    \param freq
         \~english Sound frequence, depends on the sound... 11025 by default
         \~french Fréquence du son...11025 par défaut
    \param format
         \~english Sound format.
         \~french Format du son.
    \param format
         \~english Loop true/false
         \~french  Boucle true/false
    \param format
         \~english Repeat point
         \~french  Point de répétition
*/
void PA_PlaySoundEx2(u8 PA_Channel, const void* data, s32 length, u8 volume, int freq, s16 format, BOOL repeat, int repeatPoint);

/*! \fn void PA_PlaySoundEx(u8 PA_Channel, const void* data, s32 length, u8 volume, int freq, s16 format)
    \brief
         \~english Play a given sound effect, but chose your format
         \~french Joue une fois un son, mais en choisissant le format
    \param PA_Channel
         \~english Audio channel, from 0 to 7
         \~french Canal audio, de 0 à 7
    \param data
         \~english Sound data
         \~french Données du son
    \param length
         \~english Sound length, with (u32)sound_size
         \~french Longueur du son, avec (u32)sound_size
    \param volume
         \~english Volume, from 0 to 127. 64 if not used
         \~french Volume, de 0 à 127. 64 si rien n'est mis
    \param freq
         \~english Sound frequence, depends on the sound... 11025 by default
         \~french Fréquence du son...11025 par défaut
    \param format
         \~english Sound format. 0 8 bits 1 16 bits 2 ADPCM 3 PSG
         \~french Format du son.
*/
extern inline void PA_PlaySoundEx(u8 PA_Channel, const void* data, s32 length, u8 volume, int freq, s16 format){
PA_PlaySoundEx2(PA_Channel, data, length, volume, freq, format,false,0);
}

/*! \fn extern inline void PA_PlayGBFSSoundEx(u8 PA_Channel, u16 FS_wav_number, u8 volume, int freq, s16 format)
    \brief
         \~english Play a given sound effect, but chose your format, from GBFS
         \~french Joue une fois un son, mais en choisissant le format, depuis GBFS
    \param PA_Channel
         \~english Audio channel, from 0 to 7
         \~french Canal audio, de 0 à 7
    \param FS_wav_number
         \~english Number of your sound RAW file in the PA GBFS system
         \~french Numéro du son RAW dans PA GBFS
    \param volume
         \~english Volume, from 0 to 127. 64 if not used
         \~french Volume, de 0 à 127. 64 si rien n'est mis
    \param freq
         \~english Sound frequence, depends on the sound... 11025 by default
         \~french Fréquence du son...11025 par défaut
    \param format
         \~english Sound format.
         \~french Format du son.
*/
extern inline void PA_PlayGBFSSoundEx(u8 PA_Channel, u16 FS_wav_number, u8 volume, int freq, s16 format){
s32 length = (PA_GBFSfile[FS_wav_number].Length >> 2) + 1; // Pour etre sur...
	free(FS_wav[PA_Channel]);
	FS_wav[PA_Channel] = (u32*)malloc(length << 2);
	
	DMA_Copy(PA_GBFSfile[FS_wav_number].File, FS_wav[PA_Channel], length, DMA_32NOW);

	PA_PlaySoundEx(PA_Channel, (void*)FS_wav[PA_Channel], length << 2, volume, freq, format);
}

/*! \fn extern inline void PA_PlayGBFSSoundEx2(u8 PA_Channel, u16 FS_wav_number, u8 volume, int freq, s16 format, BOOL repeat, int repeatPoint)
    \brief
         \~english Play a given sound effect, but chose your format and how to loop it, from GBFS
         \~french Joue une fois un son, mais en choisissant le format et si vous le faites boucler, depuis GBFS
    \param PA_Channel
         \~english Audio channel, from 0 to 7
         \~french Canal audio, de 0 à 7
    \param FS_wav_number
         \~english Number of your sound RAW file in the PA GBFS system
         \~french Numéro du son RAW dans PA GBFS
    \param volume
         \~english Volume, from 0 to 127. 64 if not used
         \~french Volume, de 0 à 127. 64 si rien n'est mis
    \param freq
         \~english Sound frequence, depends on the sound... 11025 by default
         \~french Fréquence du son...11025 par défaut
    \param format
         \~english Sound format.
         \~french Format du son.
    \param format
         \~english Loop true/false
         \~french  Boucle true/false
    \param format
         \~english Repeat point
         \~french  Point de répétition
*/
extern inline void PA_PlayGBFSSoundEx2(u8 PA_Channel, u16 FS_wav_number, u8 volume, int freq, s16 format, BOOL repeat, int repeatPoint){
s32 length = (PA_GBFSfile[FS_wav_number].Length >> 2) + 1; // Pour etre sur...
	free(FS_wav[PA_Channel]);
	FS_wav[PA_Channel] = (u32*)malloc(length << 2);
	
	DMA_Copy(PA_GBFSfile[FS_wav_number].File, FS_wav[PA_Channel], length, DMA_32NOW);

	PA_PlaySoundEx2(PA_Channel, (void*)FS_wav[PA_Channel], length << 2, volume, freq, format, repeat, repeatPoint);

}


/*! \fn extern inline void PA_PlayFSSoundEx(u8 PA_Channel, u16 PAFS_wav_number, u8 volume, int freq, s16 format)
    \brief
         \~english Play a given sound effect, but chose your format, from PAFS
         \~french Joue une fois un son, mais en choisissant le format, depuis PAFS
    \param PA_Channel
         \~english Audio channel, from 0 to 7
         \~french Canal audio, de 0 à 7
    \param PAFS_wav_number
         \~english Number of your sound RAW file in the PA GBFS system
         \~french Numéro du son RAW dans PA GBFS
    \param volume
         \~english Volume, from 0 to 127. 64 if not used
         \~french Volume, de 0 à 127. 64 si rien n'est mis
    \param freq
         \~english Sound frequence, depends on the sound... 11025 by default
         \~french Fréquence du son...11025 par défaut
    \param format
         \~english Sound format.
         \~french Format du son.
*/
extern inline void PA_PlayFSSoundEx(u8 PA_Channel, u16 PAFS_wav_number, u8 volume, int freq, s16 format){
	PA_Malloc((void**)FS_wav[PA_Channel], PA_FSFile[PAFS_wav_number].Length+4);

	s32 i;
	s32 length = (PA_FSFile[PAFS_wav_number].Length>>2) + 1; // Pour etre sur...
	for (i = 0; i < length; i++) FS_wav[PA_Channel][i] = ((u32*)PA_PAFSFile(PAFS_wav_number))[i];
	
	PA_PlaySoundEx(PA_Channel, (void*)FS_wav[PA_Channel], length << 2, volume, freq, format);
}

/*! \fn extern inline void PA_PlayFSSoundEx2(u8 PA_Channel, u16 PAFS_wav_number, u8 volume, int freq, s16 format, BOOL repeat, int repeatPoint)
    \brief
         \~english Play a given sound effect, but chose your format, from PAFS
         \~french Joue une fois un son, mais en choisissant le format, depuis PAFS
    \param PA_Channel
         \~english Audio channel, from 0 to 7
         \~french Canal audio, de 0 à 7
    \param PAFS_wav_number
         \~english Number of your sound RAW file in the PA GBFS system
         \~french Numéro du son RAW dans PA GBFS
    \param volume
         \~english Volume, from 0 to 127. 64 if not used
         \~french Volume, de 0 à 127. 64 si rien n'est mis
    \param freq
         \~english Sound frequence, depends on the sound... 11025 by default
         \~french Fréquence du son...11025 par défaut
    \param format
         \~english Sound format.
         \~french Format du son.
    \param format
         \~english Loop true/false
         \~french  Boucle true/false
    \param format
         \~english Repeat point
         \~french  Point de répétition
*/
extern inline void PA_PlayFSSoundEx2(u8 PA_Channel, u16 PAFS_wav_number, u8 volume, int freq, s16 format, BOOL repeat, int repeatPoint){
	PA_Malloc((void**)FS_wav[PA_Channel], PA_FSFile[PAFS_wav_number].Length+4);

	s32 i;
	s32 length = (PA_FSFile[PAFS_wav_number].Length>>2) + 1; // Pour etre sur...
	for (i = 0; i < length; i++) FS_wav[PA_Channel][i] = ((u32*)PA_PAFSFile(PAFS_wav_number))[i];
	
	PA_PlaySoundEx2(PA_Channel, (void*)FS_wav[PA_Channel], length << 2, volume, freq, format, repeat, repeatPoint);
}

/*! \fn extern inline void PA_PlayFSStreamSoundEx2(u8 PA_Channel, u16 PAFS_wav_number, u8 volume, int freq, s16 format, BOOL repeat, int repeatPoint)
    \brief
         \~english Play a given sound effect, but chose your format and how to loop it, from PAFS with a stream effect
         \~french Joue une fois un son, mais en choisissant le format et si vous le faites boucler, depuis PAFS avec un effet de streaming
    \param PA_Channel
         \~english Audio channel, from 0 to 7
         \~french Canal audio, de 0 à 7
    \param PAFS_wav_number
         \~english Number of your sound RAW file in the PA GBFS system
         \~french Numéro du son RAW dans PA GBFS
    \param volume
         \~english Volume, from 0 to 127. 64 if not used
         \~french Volume, de 0 à 127. 64 si rien n'est mis
    \param freq
         \~english Sound frequence, depends on the sound... 11025 by default
         \~french Fréquence du son...11025 par défaut
    \param format
         \~english Sound format.
         \~french Format du son.
    \param format
         \~english Loop true/false
         \~french  Boucle true/false
    \param format
         \~english Repeat point
         \~french  Point de répétition
*/

extern void PA_StopSound(u8 PA_Channel);
extern inline void PA_PlayFSStreamSoundEx2(u8 PA_Channel, u16 PAFS_wav_number, u8 volume, int freq, s16 format, BOOL repeat, int repeatPoint);

/*! \fn extern inline void PA_PlaySound(u8 PA_Channel, const void* data, s32 length, u8 volume, u32 freq)
    \brief
         \~english Play a given sound effect, with default format (raw)
         \~french Joue une fois un son, avec format par défaut (raw)
    \param PA_Channel
         \~english Audio channel, from 0 to 7
         \~french Canal audio, de 0 à 7
    \param data
         \~english Sound data
         \~french Données du son
    \param length
         \~english Sound length, with (u32)sound_size
         \~french Longueur du son, avec (u32)sound_size
    \param volume
         \~english Volume, from 0 to 127. 64 if not used
         \~french Volume, de 0 à 127. 64 si rien n'est mis
    \param freq
         \~english Sound frequence, depends on the sound... 11025 by default
         \~french Fréquence du son...11025 par défaut
*/
extern inline void PA_PlaySound(u8 PA_Channel, const void* data, s32 length, u8 volume, u32 freq){
PA_PlaySoundEx2(PA_Channel, data, length, volume, freq, 1,false,0);
}

/*! \fn extern inline void PA_PlayGBFSSound(u8 PA_Channel, u16 FS_wav_number, u8 volume, u32 freq)
    \brief
         \~english Play a given sound effect, with default format (raw), from GBFS
         \~french Joue une fois un son, avec format par défaut (raw), depuis GBFS
    \param PA_Channel
         \~english Audio channel, from 0 to 7
         \~french Canal audio, de 0 à 7
    \param FS_wav_number
         \~english Number of your sound RAW file in the PA GBFS system
         \~french Numéro du son RAW dans PA GBFS
    \param volume
         \~english Volume, from 0 to 127. 64 if not used
         \~french Volume, de 0 à 127. 64 si rien n'est mis
    \param freq
         \~english Sound frequence, depends on the sound... 11025 by default
         \~french Fréquence du son...11025 par défaut
*/
extern inline void PA_PlayGBFSSound(u8 PA_Channel, u16 FS_wav_number, u8 volume, u32 freq){
s32 length = (PA_GBFSfile[FS_wav_number].Length >> 2) + 1; // Pour etre sur...
	free(FS_wav[PA_Channel]);
	FS_wav[PA_Channel] = (u32*)malloc(length << 2);
	
	DMA_Copy(PA_GBFSfile[FS_wav_number].File, FS_wav[PA_Channel], length, DMA_32NOW);

	PA_PlaySound(PA_Channel, (void*)FS_wav[PA_Channel], length << 2, volume, freq);

//	SndPlay(PA_Channel, (void*)FS_wav[PA_Channel], length << 2, volume, freq, 0, false, 0);
}


/*! \fn extern inline void PA_PlayFSSound(u8 PA_Channel, u16 PAFS_wav_number, u8 volume, u32 freq)
    \brief
         \~english Play a given sound effect, with default format (raw), from PAFS
         \~french Joue une fois un son, avec format par défaut (raw), depuis PAFS
    \param PA_Channel
         \~english Audio channel, from 0 to 7
         \~french Canal audio, de 0 à 7
    \param PAFS_wav_number
         \~english Number of your sound RAW file in the PA GBFS system
         \~french Numéro du son RAW dans PA GBFS
    \param volume
         \~english Volume, from 0 to 127. 64 if not used
         \~french Volume, de 0 à 127. 64 si rien n'est mis
    \param freq
         \~english Sound frequence, depends on the sound... 11025 by default
         \~french Fréquence du son...11025 par défaut
*/
extern inline void PA_PlayFSSound(u8 PA_Channel, u16 PAFS_wav_number, u8 volume, u32 freq){
	PA_Malloc((void**)(FS_wav[PA_Channel]), PA_FSFile[PAFS_wav_number].Length+4);

	s32 i;
	s32 length = (PA_FSFile[PAFS_wav_number].Length>>2) + 1; // Pour etre sur...
	for (i = 0; i < length; i++) FS_wav[PA_Channel][i] = ((u32*)PA_PAFSFile(PAFS_wav_number))[i];
	
	PA_PlaySound(PA_Channel, (void*)FS_wav[PA_Channel], length << 2, volume, freq);
}



/*! \def PA_PlaySimpleSound(PA_Channel, sound)
    \brief
         \~english Simplest sound playing function... Takes the default options for volume, format, and rate (11025). You can change these options by using PA_SetDefaultSound
         \~french Fonction la plus simple pour jouer un son... Utiliser les options par défaut pour le volume, le format, et la fréquence (11025). On peut changer ces options avec PA_SetDefaultSound
    \param PA_Channel
         \~english Audio channel, from 0 to 7
         \~french Canal audio, de 0 à 7
    \param sound
         \~english Sound name...
         \~french Nom du son...
*/

#define PA_PlaySimpleSound(PA_Channel, sound) PA_PlaySoundEx(PA_Channel, (void*)sound, (u32)sound##_size, PA_SoundOption.volume, PA_SoundOption.freq, PA_SoundOption.format)


/*! \fn extern inline void PA_PlayGBFSSimpleSound(u8 PA_Channel, u16 FS_wav_number)
    \brief
         \~english Simplest sound playing function... From GBFS... Takes the default options for volume, format, and rate (11025). You can change these options by using PA_SetDefaultSound
         \~french Fonction la plus simple pour jouer un son... Depuis GBFS... Utiliser les options par défaut pour le volume, le format, et la fréquence (11025). On peut changer ces options avec PA_SetDefaultSound
    \param PA_Channel
         \~english Audio channel, from 0 to 7
         \~french Canal audio, de 0 à 7
    \param FS_wav_number
         \~english Number of your sound RAW file in the PA GBFS system
         \~french Numéro du son RAW dans PA GBFS
*/
extern inline void PA_PlayGBFSSimpleSound(u8 PA_Channel, u16 FS_wav_number){
s32 length = (PA_GBFSfile[FS_wav_number].Length >> 2) + 1; // Pour etre sur...
	free(FS_wav[PA_Channel]);
	FS_wav[PA_Channel] = (u32*)malloc(length << 2);
	
	DMA_Copy(PA_GBFSfile[FS_wav_number].File, FS_wav[PA_Channel], length, DMA_32NOW);

	PA_PlaySound(PA_Channel, (void*)FS_wav[PA_Channel], length << 2, PA_SoundOption.volume, PA_SoundOption.freq);
}




/*! \fn extern inline void PA_PlayFSSimpleSound(u8 PA_Channel, u16 PAFS_wav_number)
    \brief
         \~english Simplest sound playing function... From PAFS... Takes the default options for volume, format, and rate (11025). You can change these options by using PA_SetDefaultSound
         \~french Fonction la plus simple pour jouer un son... Depuis PAFS... Utiliser les options par défaut pour le volume, le format, et la fréquence (11025). On peut changer ces options avec PA_SetDefaultSound
    \param PA_Channel
         \~english Audio channel, from 0 to 7
         \~french Canal audio, de 0 à 7
    \param PAFS_wav_number
         \~english Number of your sound RAW file in the PA GBFS system
         \~french Numéro du son RAW dans PA GBFS
*/
extern inline void PA_PlayFSSimpleSound(u8 PA_Channel, u16 PAFS_wav_number){
	PA_Malloc((void**)(FS_wav[PA_Channel]), PA_FSFile[PAFS_wav_number].Length+4);

	s32 i;
	s32 length = (PA_FSFile[PAFS_wav_number].Length>>2) + 1; // Pour etre sur...
	for (i = 0; i < length; i++) FS_wav[PA_Channel][i] = ((u32*)PA_PAFSFile(PAFS_wav_number))[i];
	
	PA_PlaySound(PA_Channel, (void*)FS_wav[PA_Channel], PA_FSFile[PAFS_wav_number].Length, PA_SoundOption.volume, PA_SoundOption.freq);
}






/*! \def PA_PlayMod(mod_snd)
    \brief
         \~english Play a mod ! Thanks to Deku for that :p
         \~french Jouer un mod ! Merci à Deku pour le mod player !
    \param mod_snd
         \~english Mod name
         \~french Nom du mod
*/
#define PA_PlayMod(mod_snd) SndPlayMOD((u8*)mod_snd)


/*! \fn extern inline void PA_PlayGBFSMod(u16 FS_mod_number)
    \brief
         \~english Play a mod from GBFS... Warning, it copies to RAM, so big mods will make the DS crash...
         \~french Joue un mod à partir de GBFS... Attention, ca le copie dans la RAM, donc un trop gros mod fera planter la DS
    \param FS_mod_number
         \~english Mod's PA GBFS file number
         \~french Numéro de fichier PA GBFS du mod que l'on veut jouer
*/
extern inline void PA_PlayGBFSMod(u16 FS_mod_number){
s32 length = (PA_GBFSfile[FS_mod_number].Length >> 2) + 1; // Pour etre sur...
	free(FS_mod);
	FS_mod = (u32*)malloc(length << 2);

	DMA_Copy(PA_GBFSfile[FS_mod_number].File, FS_mod, length, DMA_32NOW);

	PA_PlayMod(FS_mod);
}


/*! \fn extern inline void PA_PlayFSMod(u16 PAFS_mod_number)
    \brief
         \~english Play a mod from PAFS... Warning, it copies to RAM, so big mods will make the DS crash...
         \~french Joue un mod à partir de PAFS... Attention, ca le copie dans la RAM, donc un trop gros mod fera planter la DS
    \param FS_mod_number
         \~english Mod's PA GBFS file number
         \~french Numéro de fichier PA GBFS du mod que l'on veut jouer
*/
extern inline void PA_PlayFSMod(u16 PAFS_mod_number){
	PA_Malloc((void**)((u32)(&FS_mod)), PA_FSFile[PAFS_mod_number].Length+4);

	s32 i;
	s32 length = (PA_FSFile[PAFS_mod_number].Length>>2) + 1; // Pour etre sur...
	for (i = 0; i < length; i++) FS_mod[i] = ((u32*)PA_PAFSFile(PAFS_mod_number))[i];
	
	PA_PlayMod(FS_mod);
}



/*! \def PA_StopMod()
    \brief
         \~english Stop playing the mod ! Thanks to Deku for that :p
         \~french Arrêter de jouer le mod ! Merci à Deku pour le mod player !
*/
#define PA_StopMod() SndStopMOD()


/*! \def PA_PauseMod(u8)
    \brief
         \~english Pause the mod ! Thanks to Deku for that :p
         \~french Mettre le mod en pause ! Merci à Deku pour le mod player !
    \param u8
         \~english 0 or 1...
         \~french 0 ou 1...
*/
#define PA_PauseMod(u8) SndPauseMOD(u8)



/*! \fn extern void PA_UnpauseStream(u8 PA_Channel)
    \brief
         \~english Stop a Stream
         \~french Stoppe un stream
    \param PA_Channel
         \~english Audio channel, from 0 to 15
         \~french Canal audio, de 0 à 15
*/
void PA_StopStream(u8 PA_Channel);

/*! \fn extern void PA_UnpauseStream(u8 PA_Channel)
    \brief
         \~english Pause a Stream
         \~french Met en pause un stream
    \param PA_Channel
         \~english Audio channel, from 0 to 15
         \~french Canal audio, de 0 à 15
*/
void PA_PauseStream(u8 PA_Channel);

/*! \fn extern void PA_UnpauseStream(u8 PA_Channel)
    \brief
         \~english Unpause a Stream
         \~french Redémarre un stream
    \param PA_Channel
         \~english Audio channel, from 0 to 15
         \~french Canal audio, de 0 à 15
*/
void PA_UnpauseStream(u8 PA_Channel);


/*! \fn extern inline u8 PA_SoundChannelIsBusy(u8 PA_Channel)
    \brief
         \~english Check if a channel is busy...
         \~french Vérifie si un canal est occupé...
    \param PA_Channel
         \~english Audio channel, from 0 to 15
         \~french Canal audio, de 0 à 15
*/
extern inline u8 PA_SoundChannelIsBusy(u8 PA_Channel)	{
	return ((volatile u8)PA_IPC.Sound[PA_Channel].Busy);
}

/*! \fn extern inline s8 PA_GetFreeSoundChannel(void)
    \brief
         \~english Get the first available channel
         \~french Récupérer le premier canal disponible
*/
extern inline s8 PA_GetFreeSoundChannel(void){
	u8 i;
	for (i = 0; i < 16; i++) if (!PA_SoundChannelIsBusy(i)) return i;
	
	return -1;
}


/*! \fn extern inline void PA_SetSoundChannelVol(u8 PA_Channel, u8 Volume)
    \brief
         \~english Change the volume of a playing sound
         \~french Changer le volume d'un son en cours
    \param PA_Channel
         \~english Audio channel, from 0 to 7
         \~french Canal audio, de 0 à 7
    \param Volume
         \~english Volume, from 0 to 127. 
         \~french Volume, de 0 à 127.
*/
extern inline void PA_SetSoundChannelVol(u8 PA_Channel, u8 Volume){
	PA_IPC.Sound[PA_Channel].Volume = (Volume&127) + (1<<7);  // Volume level
}


/*! \fn extern inline void PA_SetSoundVol(u8 Volume)
    \brief
         \~english Set the master sound volume
         \~french Changer le volume global
    \param Volume
         \~english Volume, from 0 to 127. 
         \~french Volume, de 0 à 127.
*/
extern inline void PA_SetSoundVol(u8 Volume){
	PA_IPC.Sound[16].Volume = (Volume&127) + (1<<7);  // Volume level, enable change bit
}



/*! \fn extern inline void PA_SetSoundChannelPan(u8 PA_Channel, u8 Pan)
    \brief
         \~english Change the pan of a playing sound
         \~french Changer le pan d'un son en cours
    \param PA_Channel
         \~english Audio channel, from 0 to 7
         \~french Canal audio, de 0 à 7
    \param Pan
         \~english Pan, from 0 to 127. (left-right)
         \~french Pan, de 0 à 127. (gauche-droite)
*/
extern inline void PA_SetSoundChannelPan(u8 PA_Channel, u8 Pan){
	PA_IPC.Sound[PA_Channel].Pan = (Pan&127) + (1<<7);  // Pan level
}

/** @} */ // end of SoundARM9

/*! \fn extern inline void PA_PlayGBFSSoundEx2(u8 PA_Channel, u16 FS_wav_number, u8 volume, int freq, s16 format)
    \brief
         \~english Play a given sound effect, but chose your format and how to loop it, from GBFS with a stream effect
         \~french Joue une fois un son, mais en choisissant le format et si vous le faites boucler, depuis GBFS avec un effet de streaming
    \param PA_Channel
         \~english Audio channel, from 0 to 7
         \~french Canal audio, de 0 à 7
    \param FS_wav_number
         \~english Number of your sound RAW file in the PA GBFS system
         \~french Numéro du son RAW dans PA GBFS
    \param volume
         \~english Volume, from 0 to 127. 64 if not used
         \~french Volume, de 0 à 127. 64 si rien n'est mis
    \param freq
         \~english Sound frequence, depends on the sound... 11025 by default
         \~french Fréquence du son...11025 par défaut
    \param format
         \~english Sound format.
         \~french Format du son.
    \param format
         \~english Loop true/false
         \~french  Boucle true/false
    \param format
         \~english Repeat point
         \~french  Point de répétition
*/
//void PA_PlayGBFSStreamSoundEx2(u8 PA_Channel, u16 FS_wav_number, u8 volume, int freq, s16 format, BOOL repeat, int repeatPoint);



/*! \fn extern inline void PA_MicStartRecording(u8* buffer, int length)
    \brief
         \~english Start recording
         \~french Démarre l'enregistrement
    \param buffer
         \~english recording buffer
         \~french buffer d'enregistrement
    \param length
         \~english Buffer length must be dividable by 1600
         \~french Longueur du buffer doit être divisible par 1600
*/
//extern inline void PA_MicStartRecording(u8* buffer, int length){StartRecording(buffer, length);}

/*! \fn extern inline void PA_MicStopRecording()
    \brief
         \~english Stop Recording
         \~french Arrete l'enregistrement
*/
//#define PA_MicStopRecording() StopRecording()



#endif
