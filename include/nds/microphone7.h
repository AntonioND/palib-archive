#if !defined(MICROPHONE7_H)
#define MICROPHONE7_H

#ifdef __cplusplus
extern "C" {
#endif

/*
   microphone code based on neimod's microphone example.
   See: http://neimod.com/dstek/ 
   Chris Double (chris.double@double.co.nz)
   http://www.double.co.nz/nintendo_ds
*/

/* Turn the microphone on */
void TurnOnMicrophone();

/* Turn the microphone off */
void TurnOffMicrophone();

/* Fill the buffer with data from the microphone. The buffer will be
   signed sound data at 16kHz. Once the length of the buffer is
   reached, no more data will be stored. Uses ARM7 timer 0.  
*/
void PA_StartRecording(s8* buffer, int length);

/* Stop recording data, and return the length of data recorded. */
int PA_StopRecording();

/* This must be called during IRQ_TIMER0 */
void PA_ProcessMicrophoneTimerIRQ();

#ifdef __cplusplus
}
#endif

#endif
