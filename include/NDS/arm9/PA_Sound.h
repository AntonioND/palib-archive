#ifndef _PA_Sound9
#define _PA_Sound9

/*! \file PA_Sound.h
    \brief Sound info

    Play sounds ! Arm9 only
*/


#include "../Sound9.h"
#include "PA_GBFS.h"
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

extern u8 *GBFS_mod;
extern u8 *GBFS_wav[16]; // 16 channels...

//plays an 8 bit mono sample at 11025Hz






/** @defgroup SoundARM9 Sound ARM9 functions
 *  Play sounds, etc...
 *  @{
 */


/*! \fn void PA_SetDefaultSound(u8 volume, s16 freq, s16 format)
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
void PA_SetDefaultSound(u8 volume, s16 freq, s16 format);


/*! \fn extern inline void PA_InitSound(void)
    \brief
         \~english Initialise the Sound system, for mods and sound effects
         \~french Initialiser les sons, pour le mod player et les effets sonores
*/
extern inline void PA_InitSound(void) {
	SndInit9 ();
	sndMemPool = (u32*)0x2200000;
	SndSetMemPool(sndMemPool, SND_MEM_POOL_SIZE);
	PA_SetDefaultSound(127, 11025, 1);
	GBFS_mod = (u8*)malloc(2); // Initialise a small portion of memory, will make it bigger later on...
}



/*! \fn void PA_PlaySoundEx(u8 PA_Channel, const void* data, u32 length, u8 volume, s16 freq, s16 format)
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
*/
void PA_PlaySoundEx(u8 PA_Channel, const void* data, u32 length, u8 volume, s16 freq, s16 format);

/*! \fn extern inline void PA_PlayGBFSSoundEx(u8 PA_Channel, u16 GBFS_wav_number, u8 volume, s16 freq, s16 format)
    \brief
         \~english Play a given sound effect, but chose your format, from GBFS
         \~french Joue une fois un son, mais en choisissant le format, depuis GBFS
    \param PA_Channel
         \~english Audio channel, from 0 to 7
         \~french Canal audio, de 0 à 7
    \param GBFS_wav_number
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
extern inline void PA_PlayGBFSSoundEx(u8 PA_Channel, u16 GBFS_wav_number, u8 volume, s16 freq, s16 format){
s32 length = (PA_GBFSfile[GBFS_wav_number].Length >> 2) + 1; // Pour etre sur...
	free(GBFS_wav[PA_Channel]);
	GBFS_wav[PA_Channel] = (u8*)malloc(length << 2);
	
	DMA_Copy(PA_GBFSfile[GBFS_wav_number].File, GBFS_wav[PA_Channel], length, DMA_32NOW);

	PA_PlaySoundEx(PA_Channel, (void*)GBFS_wav[PA_Channel], length << 2, volume, freq, format);
}

/*! \fn extern inline void PA_PlaySound(u8 PA_Channel, const void* data, u32 length, u8 volume, s16 freq)
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
extern inline void PA_PlaySound(u8 PA_Channel, const void* data, u32 length, u8 volume, s16 freq){
PA_PlaySoundEx(PA_Channel, data, length, volume, freq, 1);
}

/*! \fn extern inline void PA_PlayGBFSSound(u8 PA_Channel, u16 GBFS_wav_number, u8 volume, s16 freq)
    \brief
         \~english Play a given sound effect, with default format (raw), from GBFS
         \~french Joue une fois un son, avec format par défaut (raw), depuis GBFS
    \param PA_Channel
         \~english Audio channel, from 0 to 7
         \~french Canal audio, de 0 à 7
    \param GBFS_wav_number
         \~english Number of your sound RAW file in the PA GBFS system
         \~french Numéro du son RAW dans PA GBFS
    \param volume
         \~english Volume, from 0 to 127. 64 if not used
         \~french Volume, de 0 à 127. 64 si rien n'est mis
    \param freq
         \~english Sound frequence, depends on the sound... 11025 by default
         \~french Fréquence du son...11025 par défaut
*/
extern inline void PA_PlayGBFSSound(u8 PA_Channel, u16 GBFS_wav_number, u8 volume, s16 freq){
s32 length = (PA_GBFSfile[GBFS_wav_number].Length >> 2) + 1; // Pour etre sur...
	free(GBFS_wav[PA_Channel]);
	GBFS_wav[PA_Channel] = (u8*)malloc(length << 2);
	
	DMA_Copy(PA_GBFSfile[GBFS_wav_number].File, GBFS_wav[PA_Channel], length, DMA_32NOW);

	PA_PlaySound(PA_Channel, (void*)GBFS_wav[PA_Channel], length << 2, volume, freq);
}

/*! \fn extern inline void PA_PlaySimpleSound(u8 PA_Channel, const void* data, u32 length)
    \brief
         \~english Simplest sound playing function... Takes the default options for volume, format, and rate (11025). You can change these options by using PA_SetDefaultSound
         \~french Fonction la plus simple pour jouer un son... Utiliser les options par défaut pour le volume, le format, et la fréquence (11025). On peut changer ces options avec PA_SetDefaultSound
    \param PA_Channel
         \~english Audio channel, from 0 to 7
         \~french Canal audio, de 0 à 7
    \param data
         \~english Sound data
         \~french Données du son
    \param length
         \~english Sound length, with (u32)sound_size
         \~french Longueur du son, avec (u32)sound_size
*/
extern inline void PA_PlaySimpleSound(u8 PA_Channel, const void* data, u32 length){
PA_PlaySoundEx(PA_Channel, data, length, PA_SoundOption.volume, PA_SoundOption.freq, PA_SoundOption.format);
}


/*! \fn extern inline void PA_PlayGBFSSimpleSound(u8 PA_Channel, u16 GBFS_wav_number)
    \brief
         \~english Simplest sound playing function... From GBFS... Takes the default options for volume, format, and rate (11025). You can change these options by using PA_SetDefaultSound
         \~french Fonction la plus simple pour jouer un son... Depuis GBFS... Utiliser les options par défaut pour le volume, le format, et la fréquence (11025). On peut changer ces options avec PA_SetDefaultSound
    \param PA_Channel
         \~english Audio channel, from 0 to 7
         \~french Canal audio, de 0 à 7
    \param GBFS_wav_number
         \~english Number of your sound RAW file in the PA GBFS system
         \~french Numéro du son RAW dans PA GBFS
*/
extern inline void PA_PlayGBFSSimpleSound(u8 PA_Channel, u16 GBFS_wav_number){
s32 length = (PA_GBFSfile[GBFS_wav_number].Length >> 2) + 1; // Pour etre sur...
	free(GBFS_wav[PA_Channel]);
	GBFS_wav[PA_Channel] = (u8*)malloc(length << 2);
	
	DMA_Copy(PA_GBFSfile[GBFS_wav_number].File, GBFS_wav[PA_Channel], length, DMA_32NOW);

	PA_PlaySimpleSound(PA_Channel, (void*)GBFS_wav[PA_Channel], length << 2);
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


/*! \fn extern inline void PA_PlayGBFSMod(u16 GBFS_mod_number)
    \brief
         \~english Play a mod from GBFS... Warning, it copies to RAM, so big mods will make the DS crash...
         \~french Joue un mod à partir de GBFS... Attention, ca le copie dans la RAM, donc un trop gros mod fera planter la DS
    \param GBFS_mod_number
         \~english Mod's PA GBFS file number
         \~french Numéro de fichier PA GBFS du mod que l'on veut jouer
*/
extern inline void PA_PlayGBFSMod(u16 GBFS_mod_number){
s32 length = (PA_GBFSfile[GBFS_mod_number].Length >> 2) + 1; // Pour etre sur...
	free(GBFS_mod);
	GBFS_mod = (u8*)malloc(length << 2);

	DMA_Copy(PA_GBFSfile[GBFS_mod_number].File, GBFS_mod, length, DMA_32NOW);
/*	s32 i;	
	u32 *mod = (u32*)PA_GBFSfile[GBFS_mod_number].File;	
	for (i = 0; i < length; i++) // Copy the file in RAM
		GBFS_mod[i] = mod[i];*/
	PA_PlayMod(GBFS_mod);
}

/*! \def PA_StopMod()
    \brief
         \~english Stop playing the mod ! Thanks to Deku for that :p
         \~french Arrêter de jouer le mod ! Merci à Deku pour le mod player !
*/
#define PA_StopMod() SndStopMOD()


/*! \def PA_PauseMod(bool)
    \brief
         \~english Pause the mod ! Thanks to Deku for that :p
         \~french Mettre le mod en pause ! Merci à Deku pour le mod player !
    \param bool
         \~english 0 or 1...
         \~french 0 ou 1...
*/
#define PA_PauseMod(bool) SndPauseMOD(bool)


/** @} */ // end of SoundARM9




#endif


