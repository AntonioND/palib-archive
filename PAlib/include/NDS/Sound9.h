// ----- Change log -----
// 05/08/05 - Created
// ----------------------

#ifndef SOUND9_H
#define SOUND9_H

#include <nds.h>
#include "../../SoundCommon.h"

#ifdef __cplusplus
extern "C" {
#endif



// ----- Functions -----

extern void SndInit9();
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
