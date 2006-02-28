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

#define CODE_IN_IWRAM __attribute__ ((section (".iwram"), long_call))
#define VAR_IN_IWRAM __attribute__ ((section (".iwram")))

// ----- Constants -----

#define SND_MAX_CHANNELS		16
#define SND_CHN_INVALID			0xff

typedef enum _MOD_STATE
{
	MOD_STATE_STOP,
	MOD_STATE_PLAY,
	MOD_STATE_PAUSE,

} MOD_STATE;



// ----- Structures -----

typedef struct _SOUND_VARS
{
	void	*memPool;				// Reserved memory area to load songs
	u32		memPoolSize;			// Needs to be as big as your largest MOD
	u16		irqTimer;				// REG_TM0CNT_L value for MOD update IRQ
	u16		channelBlocked;			// One bit per mixer channel
	u8		curCmd;					// Index into shared sndControl.cmd[]
	BOOL	bInitialized;			// Set by SndInit

} SOUND_VARS;


typedef struct _SAMPLE_HEADER
{
		// These match up with the variables in an actual 
		// MOD file (excluding 22-byte name), for easy loading
	u16  length;
	u8   finetune;
	u8   vol;
	u16  loopStart;
	u16  loopLength;
		// This one is not in an actual MOD file
	s8   *smpData;

} SAMPLE_HEADER;

	// Used in the MOD_CHANNEL struct below, to make recycling code for 
	// the vibrato and tremolo effects easier
typedef struct _MOD_VIBRATO_PARAMS
{
	u8 speed	: 4;	// Added to tick each update
	u8 depth	: 4;	// Multiplied by table value, and shifted down
	u8 tick		: 6;	// Position in table. Full cycle is 64 ticks
	u8 waveform	: 2;	// Type of vibration. See MOD_WAVEFORM in Sound.c
	u8 noRetrig	: 1;	// If FALSE, reset tick to 0 when a new note is played
	u8 pad		: 7;	// Unused, align to 4 bytes
	u8 pad2		: 8;	// Unused, align to 4 bytes

} MOD_VIBRATO_PARAMS;

	// This is the data we need to keep track of for each channel 
	// while playing the song, stored in RAM
typedef struct _MOD_CHANNEL
{
	u16 period;				// Current period of note being played

	u8 note;				// Current note
	u8 sample;				// Last sample used on this channel
	u8 vol;					// Current volume
	u8 pan;					// Current pan (0-255)
	u8 finetune;			// Finetune level (0-15)

	u8 effect;				// Current effect (set to 0 each row)
	u8 param;				// Current param (set to 0 each row)

	s8 volslideSpeed;		// Current volume slide speed
	u8 portaSpeed;			// Current pitch slide up/down speed
	u8 tonePortaNote;		// Current note to slide toward
	u8 tonePortaSpeed;		// Speed to slide toward it
	u8 patLoopPos;			// Set to current row when an E60 effect is used
	u8 patLoopCount;		// Number of times left to loop

	union
	{
		u8 retrigTick;		// MOD ticks until note should retrigger
		u8 noteCutTick;		// MOD ticks until note should cut
		u8 noteDelayTick;	// MOD ticks until note should play
		u8 arpeggioTick;	// Cycles 0-2 for original note and arpeggio notes
	};

	MOD_VIBRATO_PARAMS vibrato;		// Vibrates frequency
	MOD_VIBRATO_PARAMS tremolo;		// Vibrates volume

} MOD_CHANNEL;

	// This is all the data for the currently playing song, in RAM
typedef struct _MOD
{
	char name[20];					// Name of song, from MOD file
	SAMPLE_HEADER sample[31];
	u8 **pattern;					// Pointer to table of pointers to patterns
	u8 order[128];					// Array of pattern numbers to play
	MOD_CALLBACK callback;			// User function called by 0xE0 effects

	u8 *rowPtr;						// Current position in current pattern, for quick access

	u8 state;						// MOD_STATE enum (stopped/playing/paused)

	u8 speed;
	u8 tick;						// When this gets to speed, process a new row and reset it to 0

	u8 curRow;						// When this gets to 64, move to the next order and reset to 0

	u8 channelCount;
	u8 orderCount;
	u8 curOrder;					// When this gets to orderCount, stop the song
	u8 nextOrder;					// Normally curOrder+1, unless an effect changes it
	u8 breakRow;					// Starting row when order changes (normally 0)
	u8 patDelay;					// Rows left to wait (normally 0)

	u8 tempo;						// In BPM (Hz = BPM*2/5)

	u8 pad;

	MOD_CHANNEL channel[SND_MAX_CHANNELS];   // Current state of each channel

} MOD;



// ----- Global vars -----

extern SOUND_CHANNEL	sndChannel[SND_MAX_CHANNELS];
extern SOUND_VARS		sndVars;
extern MOD				sndMod;

extern const u16		notePeriodTable[] VAR_IN_IWRAM;

// ----- Global functions -----

extern void SndInit7();
extern void SndVblIrq();
extern void SndTimerIrq();

// ----------

#ifdef __cplusplus
}
#endif

#endif	// SOUND7_H
