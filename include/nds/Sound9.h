// ----- Change log -----
// 05/08/05 - Created
// ----------------------

#ifndef SOUND9_H
#define SOUND9_H

#include <NDS.h>
#include "SoundCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

// ----- Functions -----

extern void SndInit9();
extern void SndSetMemPool(void *memPool, u32 memPoolSize);
extern void SndPlayMOD(const void *modFile);
extern void SndStopMOD();
extern void SndPauseMOD(BOOL bPaused);
extern void SndSetMODCallback(MOD_CALLBACK callback);
extern void SoundSendCmd(SND_COMMAND_TYPE cmdType, u32 param32);

// ----------

#ifdef __cplusplus
}
#endif

#endif	// SOUND9_H
