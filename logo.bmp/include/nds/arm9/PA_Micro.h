#ifndef _PA_MICRO
#define _PA_MICRO

#include "PA_General.h"

void PA_PlaySoundEx2(u8 PA_Channel, const void* data, s32 length, u8 volume, int freq, s16 format, BOOLEAN repeat, int repeatPoint);

/** @defgroup Micro Microphone
 * Record a sound and replay it...
 *  @{
 */


/*! \def PA_MicGetVol()
    \brief
         \~english Returns the Microphone volume
         \~french Renvoie le volume du micro
*/
#define PA_MicGetVol()	PA_IPC.Mic.Volume


/*! \fn extern inline void PA_MicStartRecording(u8 *Buffer, s32 Length)
    \brief
         \~english Start recording from the microphone. The sound is really ugly and low though :/
         \~french Commencer à enregistrer avec le microphone. Par contre, le son est super laid et faible :/
    \~\param Buffer
         \~english 8bit buffer in which to record the sound
         \~french Buffer dans lequel enregistrer le son
    \~\param Length
         \~english Buffer length
         \~french Longueur du buffer
*/
extern inline void PA_MicStartRecording(u8* buffer, s32 length)
{
	PA_IPC.Mic.Data = buffer;
	PA_IPC.Mic.Length = length;
}


/*! \fn extern inline void PA_MicReplay(u8 PA_Channel, u8 *Buffer, s32 Length)
    \brief
         \~english Play a recorded sound using aslib
         \~french Rejouer un son enregistrer
    \~\param Buffer
         \~english 8bit buffer in which the sound was recorded
         \~french Buffer dans lequel on a enregistré le son
    \~\param Length
         \~english Buffer length
         \~french Longueur du buffer
*/
extern inline void PA_MicReplay(u8* buffer, s32 length)
{
	SoundInfo snd = { 
        buffer, 
        length, 
        AS_PCM_8BIT,
        16384, 
        127, 
        64, 
        false, 
        0, 
        AS_NO_DELAY};
    AS_SoundPlay(snd);
}


/** @} */ // end of Micro



#endif
