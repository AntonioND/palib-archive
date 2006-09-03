// ----- Change log -----
// 05/08/05 - Created
// ----------------------

#ifndef SOUND9_H
#define SOUND9_H

#include <nds.h>
#include "SoundCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SND_MAX_CHANNELS	16

//channels infos
extern SOUND_CHANNEL	SndChannel[SND_MAX_CHANNELS];

// ----- Functions -----

extern void SndInit9();
//NO-MOD
extern void SndPlay(u8 PA_Channel, const void* data, s32 length, u8 volume, s16 freq, s16 format, BOOL repeat, int repeatPoint);
extern void SndStop(u8 PA_Channel);
extern void SndPause(u8 PA_Channel);
extern void SndUnpause(u8 PA_Channel);
extern void SndChgVolume(u8 PA_Channel, u8 Volume);
extern void SndChgPan(u8 PA_Channel, u8 Pan);
extern void SndChgRepeat(u8 PA_Channel, BOOL repeat, u32 RepeatPoint);
extern void SndChgLength(u8 PA_Channel, u32 Length);
extern void SndChgTimer(u8 PA_Channel, int Timer);
extern void StmPlay(u8 PA_Channel, const void* data, s32 length, u8 volume, s16 freq, s16 format, BOOL repeat, int repeatPoint);
//MIC
extern void StartRecording(u8* buffer, int length);
extern void StopRecording();
//MOD
extern void SndSetMemPool(void *memPool, u32 memPoolSize);
extern void SndPlayMOD(void *modFile);
extern void SndStopMOD();
extern void SndPauseMOD(BOOL bPaused);
extern void SndSetMODCallback(MOD_CALLBACK callback);

// ----------

#ifdef __cplusplus
}
#endif

#endif	// SOUND9_H
