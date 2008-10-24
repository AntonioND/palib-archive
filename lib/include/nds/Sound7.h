// ----- Change log -----
// 10/08/04 - Changed MOD_NO_SAMPLE from 0 to 31
//          - Added structures for MOD player
//          - Modified SndMix to take a number of samples, added MODUpdate
//            to update the music, and SndUpdate to do the deciding how many 
//            samples to mix at a time, and call MODUpdate as needed
// 05/08/05 - Ported to DS.
// ----------------------

#ifndef SOUND7_H
#define SOUND7_H

#include <nds.h>
#include "SoundCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SND_MAX_CHANNELS 16

// ----- Structures -----

typedef struct _SOUND_CHANNEL
{
	const s8	*data;
	u32			length;
	u16			loopStart;
	u16			loopLength;
	u16			timer;
	u8			vol;
	u8			pan;

} SOUND_CHANNEL;

typedef struct _SOUND_VARS
{
	void	*memPool;				// Reserved memory area to load songs
	u32		memPoolSize;			// Needs to be as big as your largest MOD
	u16		irqTimer;				// REG_TM0CNT_L value for MOD update IRQ
	u16		channelBlocked;			// One bit per mixer channel
	u8		curCmd;					// Index into shared sndControl.cmd[]
	BOOLEAN	bInitialized;			// Set by SndInit
	u8*		cmprsdSong;				// Compressed song

} SOUND_VARS;

// ----- Global vars -----

extern SOUND_CHANNEL	sndChannel[SND_MAX_CHANNELS];
extern SOUND_VARS		sndVars;

extern const u16		notePeriodTable[];

// ----- Global functions -----

extern void SndInit7();
extern void SndVblIrq();
extern void SndTimerIrq();

// ----------

#ifdef __cplusplus
}
#endif

#endif	// SOUND7_H
