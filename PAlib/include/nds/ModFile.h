// ----- Change log -----
// 05/08/05 - Created.
// ----------------------

#ifndef MODFILE_H
#define MODFILE_H

#include <nds.h>

#ifdef __cplusplus
extern "C" {
#endif



// ----- Functions -----

struct _MOD;	// Forward declaration

u32 ModFileLoad(const void *modFile, struct _MOD *dest, void *memPool, u32 memPoolSize);

// ----------
#ifdef __cplusplus
}
#endif

#endif	// MODFILE_H
