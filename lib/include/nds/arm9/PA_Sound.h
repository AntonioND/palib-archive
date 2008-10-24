#ifndef _PA_SOUND9_
#define _PA_SOUND9_

#ifdef __cplusplus
extern "C" {
#endif

#include "../Sound9.h"
#include "PA_IPC.h"
#include "PA_General.h"

// Some macros to improve compatibility to old code
#define PA_PlaySimpleSound(name)			AS_SoundDefaultPlay((u8*)name, (u32)name##_size, 127, 64, false, 0)
#define PA_PlaySoundRepeat(name)			AS_SoundDefaultPlay((u8*)name, (u32)name##_size, 127, 64, true, 0)
#define PA_SoundChannelIsBusy(channel)		IPC_Sound->chan[channel].busy
#define PA_SetSoundChannelPan(chan, pan)	AS_SetSoundPan(chan, pan)
#define PA_SetSoundChannelVol(chan, vol)	AS_SetSoundVolume(chan, vol)
#define PA_StopSound(chan)					AS_SoundStop(chan)
#define PA_SetSoundVol(vol)					AS_SetMasterVolume(vol)

#define PA_PlayMod(file) 					{modControl->module = (u8*)file; \
											modControl->command = MT_MSG_INIT;}
#define PA_StopMod()						modControl->command = MT_MSG_END

/*! \fn void PA_PlayPSG(u8 PA_Channel, u8 vol, u8 pan, s32 freq, u8 duty)
    \brief
         \~english Use the Programmable Sound Generators
         \~french Utiliser les Programmable Sound Generators
    \param PA_Channel
         \~english Audio channel, from 8 to 15
         \~french Canal audio, de 8 à 15
    \param vol
         \~english Volume (0-127)
         \~french Volume (0-127)	 		 
    \param freq
         \~english Frenquency
         \~french Fréquence
    \param duty
         \~english Duty (0-7)
         \~french Duty (0-7)		 
*/
void PA_PlayPSG(u8 PA_Channel, u8 vol, u8 pan, s32 freq, u8 duty);

extern void PA_StopSound(u8 PA_Channel);

#ifdef __cplusplus
}
#endif

#endif
