#ifndef _PA_MICRO
#define _PA_MICRO

#include "PA_General.h"



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
    \param Buffer
         \~english 8bit buffer in which to record the sound
         \~french Buffer dans lequel enregistrer le son
    \param Length
         \~english Buffer length
         \~french Longueur du buffer
*/
extern inline void PA_MicStartRecording(u8 *Buffer, s32 Length){
			PA_IPC.Mic.Data = Buffer; // Buffer
			PA_IPC.Mic.Length = Length; // Length
}


/*! \fn extern inline void PA_MicReplay(u8 PA_Channel, u8 *Buffer, s32 Length)
    \brief
         \~english Play a recorded sound
         \~french Rejouer un son enregistrer
    \param PA_Channel
         \~english Sound channel (0-15)
         \~french Canal audio (0-15)	 
    \param Buffer
         \~english 8bit buffer in which the sound was recorded
         \~french Buffer dans lequel on a enregistré le son
    \param Length
         \~english Buffer length
         \~french Longueur du buffer
*/
extern inline void PA_MicReplay(u8 PA_Channel, u8 *Buffer, s32 Length){
	PA_PlaySound(PA_Channel, Buffer, Length, 127, 16384/2);
}


/** @} */ // end of Micro



#endif
