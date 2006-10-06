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

typedef struct {
	s32 Rate; // Frequence
	u8* Raw;  // Pointeur vers le son
	u32 Length; // Durée du son
}PA_SoundInfo;

extern PA_SoundInfo PA_Sound[MAX_SOUNDS];  

extern u8 PA_SoundOk;

typedef struct{
	s32 Sound;
	u8 State, Action; // 0 pour stopper, 1 pour démarrer, 2 pour en lecture
	u8 Volume;
} StartSoundInfo;

extern StartSoundInfo* PA_StartSoundInfo; // Permet de démarrer ou arreter le son

/*! \file PA_Sound.h
    \brief Sound info

    Play sounds ! Arm7 only
*/

void PA_SetDSLiteBrightness(u8 level);



extern inline void PA_InitSoundSystem(void) {

//	while(i != 2) i = IPC->mailRead; // On attend que ce soit à lire
	PA_StartSoundInfo = (StartSoundInfo*)IPC->mailData;
	IPC->mailRead = 0;
	PA_SoundOk = 1; // initialisé

	
	// Turn on sound
	powerON(POWER_SOUND);
	
	//SOUND_CR = SCHANNEL_ENABLE | SOUND_VOL(0x7F);
}


/** @defgroup SoundARM7 Sound ARM7 functions
 *  Initialise sounds, etc...
 *  @{
 */


/*! \def PA_InitSimpleSound(Sound_Num, Rate, Data)
    \brief
         \~english Initialize a sound to be used later on, easy way
         \~french Initialiser un son à utiliser par la suite, de manière simple
    \param Sound_Num
         \~english Sound number, from 0 to 127
         \~french Numéro du son, de 0 à 127
    \param Rate
         \~english Rate...
         \~french Frequence
    \param Data
         \~english Sound name
         \~french Nom du son
*/

#define PA_InitSimpleSound(Sound_Num, Rate, Data)  PA_InitSound(Sound_Num, Rate , (u8*)GETRAW(Data), GETRAWSIZE(Data) >> 2)



/*! \fn void PA_InitSound(s32 Sound_Num, s32 Rate, u8* Data, u32 Length)
    \brief
         \~english Initialize a sound to be used later on, complicated way
         \~french Initialiser un son à utiliser par la suite, de manière compliquée
    \param Sound_Num
         \~english Sound number, from 0 to 127
         \~french Numéro du son, de 0 à 127
    \param Rate
         \~english Rate...
         \~french Frequence
    \param Data
         \~english Sound name
         \~french Nom du son
    \param Length
         \~english Length (1/4 of the file size)
         \~french Taille (1/4 de la taille)
*/
void PA_InitSound(s32 Sound_Num, s32 Rate, u8* Data, u32 Length);




/*! \fn void PA_StartSound(u8 Channel, s32 Sound_Num, u8 Volume)
    \brief
         \~english Start a given sound
         \~french Initialiser un son à utiliser par la suite, de manière compliquée
    \param Channel
         \~english Sound channel to use, from 0 to 15
         \~french Canal à utiliser, de 0 à 15
    \param Sound_Num
         \~english Sound number, from 0 to 127
         \~french Numéro du son, de 0 à 127
    \param Volume
         \~english Volume for that sound, 0-127
         \~french Volume pour ce son, 0-127
*/
void PA_StartSound(u8 Channel, s32 Sound_Num, u8 Volume);



/*! \fn extern inline void PA_StopSound(u8 Channel)
    \brief
         \~english Stop a given sound channel
         \~french Arreter un canal
    \param Channel
         \~english Sound channel to use, from 0 to 15
         \~french Canal à utiliser, de 0 à 15
*/
extern inline void PA_StopSound(u8 Channel){
	SCHANNEL_CR(Channel) = 0;

	PA_StartSoundInfo[Channel].State = 0; // Rien
	PA_StartSoundInfo[Channel].Action = 0;
}
	




/*! \fn extern inline void PA_SetSoundVolume(u8 Volume)
    \brief
         \~english Set the global sound volume
         \~french Régler le volume global de la DS
    \param Volume
         \~english Volume, 0-127
         \~french Volume, 0-127
*/
extern inline void PA_SetSoundVolume(u8 Volume) {
	SOUND_CR &= ~SOUND_VOL(0x7F);
	SOUND_CR |= SOUND_VOL(Volume&127);
}



/*! \fn extern inline void PA_SetChannelVolume(u8 Channel, u8 Volume)
    \brief
         \~english Set the channel sound volume
         \~french Régler le volume d'un canal
    \param Channel
         \~english Sound channel to use, from 0 to 15
         \~french Canal à utiliser, de 0 à 15
    \param Volume
         \~english Volume, 0-127
         \~french Volume, 0-127
*/
extern inline void PA_SetChannelVolume(u8 Channel, u8 Volume) {
	SCHANNEL_CR(Channel) &= ~SOUND_VOL(127);
	SCHANNEL_CR(Channel) |= SOUND_VOL(Volume&127);
}




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



/** @} */ // end of SoundARM7



#ifdef __cplusplus
}
#endif





#endif


