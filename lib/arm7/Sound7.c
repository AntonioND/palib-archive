
// ----- Change log -----
// ----- Change log -----
// 10/08/04 - Modified mixer to take any number of samples to mix, 
//            rather than strictly a full buffer, added MOD updating 
//            functions, and SndUpdate to mix sound and update MOD as 
//            needed for a full frame
// 10/21/04 - Moved MOD_NO_NOTE/MOD_NO_SAMPLE defines here, 
//            since nothing outside player needs to know them
// 05/05/05 - Converted to use periods instead of Hz.
//			  MOD timing is done fixed-point, mixer moved to IWRAM.
//			  Added SFX functions.
// 05/06/05 - Added all effect functions.
// 05/08/05 - Ported to DS.
// ----------------------

#include <nds.h>
#include <string.h>
#include <malloc.h>

#include "ModFile.h"
#include "PA_Sound.h"
#include "Sound7.h"
//#include "microphone7.h"

#include "SoundCommon.h"

// ----- Constants -----

	// Switch this on if your IRQ stack isn't big enough, and you can't 
	// make it any bigger. It will use some assembly code to switch the 
	// CPU into system mode during the MOD updating interrupt, thus 
	// working on the large user stack rather than the small IRQ stack.
#define SWITCH_TO_USER_STACK	FALSE

	// For patterns to specify that there is no note. We have 5 octaves, 
	// so only notes 0-59 are used, and 63 is the highest that still fits 
	// in the same number of bits
#define MOD_NO_NOTE			63
	// Valid samples are 0-30, use 31 as a blank that still fits in 5 bits
#define MOD_NO_SAMPLE		31

#define MOD_DEFAULT_SPEED	6			// Starting speed is 6 ticks per row
#define MOD_DEFAULT_TEMPO	125			// Starting tempo is 125 bpm
#define MOD_PATTERN_ROWS	64			// All patterns are 64 rows

#define MOD_MAX_NOTE		59			// Highest valid note
#define MOD_PERIOD_MAX		1814
#define MOD_PERIOD_MIN		53

	// This is basePeriod*baseFreq, about 428*8363
#define AMIGA_VAL			3579545
	// This is 16777216/AMIGA_VAL, left shifted 17 more bits for accuracy
#define TIMER_VAL			614361

typedef enum _MOD_UPDATE_FLAGS
{
	MOD_UPD_FLG_SET_VOL		= BIT(0),	// Row or non-row, recalculates mix chn volume
	MOD_UPD_FLG_SET_FREQ	= BIT(1),	// Row or non-row, recalculates mix chn increment
	MOD_UPD_FLG_PLAY_NOTE	= BIT(2),	// Used on row-ticks to play or skip note

} MOD_UPDATE_FLAGS;

typedef enum _MOD_EFFECT_TABLE
{
	MOD_EFFECT_TABLE_ROW,
	MOD_EFFECT_TABLE_MID,

	MOD_EFFECT_TABLE_NUM,

} MOD_EFFECT_TABLE;

typedef enum _MOD_WAVEFORM
{
	MOD_WAVEFORM_SINE,
	MOD_WAVEFORM_RAMP,
	MOD_WAVEFORM_SQUARE,
	MOD_WAVEFORM_RANDOM,

	MOD_WAVEFORM_NUM,

} MOD_WAVEFORM;


// ----- Structures -----

typedef struct _MOD_UPDATE_VARS
{
	MOD_CHANNEL		*modChn;
	SOUND_CHANNEL	*sndChn;

	u8				note;
	u8				sample;
	u8				effect;
	u8				param;

	u8				chnIdx;
	u8				updateFlags;
	s8				fineSlide;
	u8				smpOffset;
	s8				vibratoSlide;
	s8				tremoloSlide;

} MOD_UPDATE_VARS;

typedef void (*EFFECT_FUNC_PTR)(MOD_UPDATE_VARS *vars);
typedef void (*COMMAND_FUNC_PTR)(SND_COMMAND *cmd);


// ----- Local function prototypes -----

	//Non - MOD functions
static void SndChgVolume		(SND_COMMAND *cmd);
static void SndChgPan			(SND_COMMAND *cmd);
static void SndChgTimer		(SND_COMMAND *cmd);
static void SndChgRepeat		(SND_COMMAND *cmd);
static void SndChgLength		(SND_COMMAND *cmd);
static void SngCmdPlay			(SND_COMMAND *cmd);
static void SngCmdStop			(SND_COMMAND *cmd);
static void SngCmdPause		(SND_COMMAND *cmd);
static void FlashChannel		(u8 channel);
/*
	//Mic functions
static void MicCmdStart		(SND_COMMAND *cmd);
static void MicCmdStop			(SND_COMMAND *cmd);
*/
	// Command functions (communication from ARM9)
static void SndCmdSetMemPool	(SND_COMMAND *cmd);
static void SndCmdPlaySong		(SND_COMMAND *cmd);
static void SndCmdStopSong		(SND_COMMAND *cmd);
static void SndCmdPauseSong		(SND_COMMAND *cmd);
static void SndCmdSetCallback	(SND_COMMAND *cmd);

	// MOD functions
static void MODStartTimer		();
static void MODStopTimer		();
static void MODPlay				(const void *modFile);
static void MODStop				();
static BOOL MODSeek				(u32 order, u32 row);
void MODUpdate					();
static void MODProcessRow		();
static void MODUpdateEffects	();
static void MODPlayNote			(MOD_UPDATE_VARS *vars);
static void MODHandleUpdateFlags(MOD_UPDATE_VARS *vars);
static void MODSetTempo			(u32 tempo);
static u32  MODGetNotePeriod	(u32 note, u32 finetune);

	// Effect helper functions
static u32  MODVolumeSlide		(u32 volume, s32 slide);
static s32  MODPitchSlide		(s32 frequency, s32 slide);
static void MODInitVibrato		(MOD_UPDATE_VARS *vars, MOD_VIBRATO_PARAMS *vibrato);
static s8   MODUpdateVibrato	(MOD_VIBRATO_PARAMS *vibrato);

	// Effect functions
static void MODFXSetVol			(MOD_UPDATE_VARS *vars);
static void MODFXSpeed			(MOD_UPDATE_VARS *vars);
static void MODFXPan			(MOD_UPDATE_VARS *vars);
static void MODFXSampleOffset	(MOD_UPDATE_VARS *vars);
static void MODFXJumpToOrder	(MOD_UPDATE_VARS *vars);
static void MODFXBreakToRow		(MOD_UPDATE_VARS *vars);
static void MODFXArpeggioRow	(MOD_UPDATE_VARS *vars);
static void MODFXArpeggioMid	(MOD_UPDATE_VARS *vars);
static void MODFXVolslideRow	(MOD_UPDATE_VARS *vars);
static void MODFXVolslideMid	(MOD_UPDATE_VARS *vars);
static void MODFXPortaRow		(MOD_UPDATE_VARS *vars);
static void MODFXPortaDownMid	(MOD_UPDATE_VARS *vars);
static void MODFXPortaUpMid		(MOD_UPDATE_VARS *vars);
static void MODFXTonePortaRow	(MOD_UPDATE_VARS *vars);
static void MODFXTonePortaMid	(MOD_UPDATE_VARS *vars);
static void MODFXVibratoRow		(MOD_UPDATE_VARS *vars);
static void MODFXVibratoMid		(MOD_UPDATE_VARS *vars);
static void MODFXTremoloRow		(MOD_UPDATE_VARS *vars);
static void MODFXTremoloMid		(MOD_UPDATE_VARS *vars);
static void MODFXVSldTPortaRow	(MOD_UPDATE_VARS *vars);
static void MODFXVSldTPortaMid	(MOD_UPDATE_VARS *vars);
static void MODFXVSldVibratoRow	(MOD_UPDATE_VARS *vars);
static void MODFXVSldVibratoMid	(MOD_UPDATE_VARS *vars);
static void MODFXSpecialRow		(MOD_UPDATE_VARS *vars);
static void MODFXSpecialMid		(MOD_UPDATE_VARS *vars);


// ----- Variables -----

SOUND_CHANNEL	sndChannel[SND_MAX_CHANNELS];
SOUND_VARS		sndVars;
MOD				sndMod;

// ----- Tables -----

   // Save ourselves the trouble of initializing each member in code
static const MOD_UPDATE_VARS modDefaultVars[MOD_EFFECT_TABLE_NUM] =
{
	{   // MOD_EFFECT_TABLE_ROW
		NULL,					// modChn
		NULL,					// sndChn
		MOD_NO_NOTE,			// note
		MOD_NO_SAMPLE,			// sample
		0,						// effect
		0,						// param
		0,						// chnIdx
		MOD_UPD_FLG_PLAY_NOTE,	// updateFlags
		0,						// fineSlide
		0,						// smpOffset
		0,						// vibratoSlide
		0,						// tremoloSlide
	},
	{   // MOD_EFFECT_TABLE_MID
		NULL,					// modChn
		NULL,					// sndChn
		MOD_NO_NOTE,			// note
		MOD_NO_SAMPLE,			// sample
		0,						// effect
		0,						// param
		0,						// chnIdx
		0,						// updateFlags
		0,						// fineSlide
		0,						// smpOffset
		0,						// vibratoSlide
		0,						// tremoloSlide
	}
};

static const EFFECT_FUNC_PTR modEffectTable[MOD_EFFECT_TABLE_NUM][16] = 
{
	{	// Row-tick updates
		MODFXArpeggioRow,		// 0x0: Arpeggio
		MODFXPortaRow, 			// 0x1: Porta up
		MODFXPortaRow, 			// 0x2: Porta down
		MODFXTonePortaRow, 		// 0x3: Tone porta
		MODFXVibratoRow, 		// 0x4: Vibrato
		MODFXVSldTPortaRow,		// 0x5: Volslide+Tone porta
		MODFXVSldVibratoRow,	// 0x6: Volslide+Vibrato
		MODFXTremoloRow, 		// 0x7: Tremolo
		MODFXPan, 				// 0x8: Set panning
		MODFXSampleOffset, 		// 0x9: Sample offset
		MODFXVolslideRow, 		// 0xA: Volume slide
		MODFXJumpToOrder, 		// 0xB: Jump to order
		MODFXSetVol, 			// 0xC: Set volume
		MODFXBreakToRow, 		// 0xD: Break to row
		MODFXSpecialRow, 		// 0xE: Special
		MODFXSpeed				// 0xF: Speed/Tempo
	}, 
	{	// Non row-tick updates
		MODFXArpeggioMid,		// 0x0: Arpeggio
		MODFXPortaUpMid, 		// 0x1: Porta up
		MODFXPortaDownMid, 		// 0x2: Porta down
		MODFXTonePortaMid, 		// 0x3: Tone porta
		MODFXVibratoMid, 		// 0x4: Vibrato
		MODFXVSldTPortaMid,		// 0x5: VolSlide+Tone porta
		MODFXVSldVibratoMid,	// 0x6: VolSlide+Vibrato
		MODFXTremoloMid, 		// 0x7: Tremolo
		NULL, 					// 0x8: Set panning
		NULL, 					// 0x9: Sample offset
		MODFXVolslideMid,		// 0xA: Volume slide
		NULL, 					// 0xB: Jump to order
		NULL, 					// 0xC: Set volume
		NULL, 					// 0xD: Break to row
		MODFXSpecialMid, 		// 0xE: Special
		NULL					// 0xF: Speed/Tempo
	}
};

static const COMMAND_FUNC_PTR cmdFuncTable[SND_CMD_NUM] = 
{
	SndChgVolume,				// SND_CHG_VOLUME
	SndChgPan,					// SND_CHG_PAN
	SndChgTimer,				// SND_CHG_TIMER
	SndChgRepeat,				// SND_CHG_REPEAT
	SndChgLength,				// SND_CHG_LENGTH
	SngCmdPlay,					// SNG_CMD_PLAY
	SngCmdStop,					// SNG_CMD_STOP
	SngCmdPause,				// SNG_CMD_PAUSE
	SndCmdSetMemPool,			// SND_CMD_SETMEMPOOL
	SndCmdPlaySong,				// SND_CMD_PLAYSONG
	SndCmdStopSong,				// SND_CMD_STOPSONG
	SndCmdPauseSong,			// SND_CMD_PAUSESONG
	SndCmdSetCallback,			// SND_CMD_SETCALLBACK
//	MicCmdStart,				// MIC_CMD_START
//	MicCmdStop,					// MIC_CMD_STOP
};

const u16 notePeriodTable[] =
{
		// Finetune 0
	 1712,  1616,  1524,  1440,  1356,  1280,  1208,  1140,  1076,  1016,   960,   906, 	// C0-B0
	  856,   808,   762,   720,   678,   640,   604,   570,   538,   508,   480,   453, 	// C1-B1
	  428,   404,   381,   360,   339,   320,   302,   285,   269,   254,   240,   226, 	// C2-B2
	  214,   202,   190,   180,   169,   160,   151,   142,   134,   127,   120,   113, 	// C3-B3
	  107,   101,    95,    90,    84,    80,    75,    71,    67,    63,    60,    56, 	// C4-B4

};	// notePeriodTable

static const u8 vibratoSineTab[32] = 
{
	  0, 24, 49, 74, 97,120,141,161,
	180,197,212,224,235,244,250,253,
	255,253,250,244,235,224,212,197,
	180,161,141,120, 97, 74, 49, 24
};

static const s8 vibratoRandomTab[64] =
{
	-111,  43, -13, -65,  50,  83, 114, -58, 
	  34, -42,-101, 127,  74, -22,  25, 120, 
	 -30,  -7,  12, 104, -46,   7,  19, -97, 
	  57, -79,-115,  61, -72,   0,  90,-128,
	 -97,  -7,  12, 101, -46,   7,-100,  19, 
	  55, -80,-128,  61, -72,  90, 121,-128,
	 -30,  45, -13, 114, -69,  49,  88, -57, 
	  35, -40,-106,   0,  74, -22,  25,  39, 
};

// ----- Functions -----

// --- Sound functions ---

	// Call this once at startup
void SndInit7()
{
	s32 i;
/*	u16 oldIME;

	oldIME = REG_IME;
	REG_IME = 0;

		// Enable sound, full volume
	POWER_CR = POWER_SOUND;
	SOUND_CR = SOUND_ENABLE | 127;

	TIMER0_CR = 0;
		// My custom IRQ handler uses this, but ndslib doesn't have it so 
		// the call to SndTimerIrq is hardcoded in ARM7 main.cpp's handler.
//	irqTable[IRQTABLE_TM0] = (u32)SndTimerIrq;
	REG_IE |= IRQ_TIMER0;*/

	memset(&sndVars, 0, sizeof(sndVars));
	memset(&sndMod, 0, sizeof(sndMod));

	for(i = 0; i < SND_MAX_CHANNELS; i++)
	{
		sndChannel[i].data			= NULL;
		sndChannel[i].length		= 0;
		sndChannel[i].loopLength	= 0;
		sndChannel[i].timer			= 0;
		sndChannel[i].pan			= 0;
		sndChannel[i].vol			= 0;
	}

	sndVars.bInitialized = TRUE;

//	REG_IME = oldIME;

}	// SndInit

void SndVblIrq()
{
	if(sndVars.bInitialized == FALSE || sndControl->bInitialized == FALSE)
		return;

	while(sndVars.curCmd != sndControl->curCmd)
	{
		SND_COMMAND *cmd = &sndControl->cmd[sndVars.curCmd];

		ASSERT(cmd->cmdType < SND_CMD_NUM);
		cmdFuncTable[cmd->cmdType](cmd);
		sndVars.curCmd++;
		sndVars.curCmd &= MAX_SND_COMMANDS-1;
	}
}

void SndTimerIrq()
{
#if !SWITCH_TO_USER_STACK
	MODUpdate();
#else
	asm volatile("	stmfd	sp!, {r0-r2}			");	// Save regs on IRQ stack.
	asm volatile("	mrs		r0, cpsr				");	// Grab CPSR, which is in IRQ mode right now.
	asm volatile("	orr		r1, r0, #0x1f			");	// 0x1f = SYS mode. No need to clear first since all bits are set.
	asm volatile("	msr		cpsr, r1				");	// Switch to SYS mode.
	asm volatile("	stmfd	sp!, {r0-r3, r12, lr}	");	// Save regs as per APCS, on user stack.
	asm volatile("	bl		MODUpdate				");	// Call the function.
	asm volatile("	ldmfd	sp!, {r0-r3, r12, lr}	");	// Load regs back from user stack.
	asm volatile("	msr		cpsr, r0				");	// Switch back to IRQ mode.
	asm volatile("	ldmfd	sp!, {r0-r1}			");	// Restore regs from IRQ stack.
#endif
}



// --- Command functions ---

static void SndCmdSetMemPool(SND_COMMAND *cmd)
{
	sndVars.memPool = (void*)cmd->param32;
	sndVars.memPoolSize = 
		cmd->param[0] | (cmd->param[1] << 8) | (cmd->param[2] << 16);
}

static void SndCmdPlaySong(SND_COMMAND *cmd)
{
	MODStop();
	MODPlay((void*)cmd->param32);
}

static void SndCmdStopSong(SND_COMMAND *cmd)
{
	MODStop();
}

static void SndCmdPauseSong(SND_COMMAND *cmd)
{
	if(cmd->param[0] == 0)
	{
		if(sndMod.state == MOD_STATE_PAUSE)
			sndMod.state = MOD_STATE_PLAY;
	}
	else
	{
		if(sndMod.state == MOD_STATE_PLAY)
			sndMod.state = MOD_STATE_PAUSE;
	}
}

static void SndCmdSetCallback(SND_COMMAND *cmd)
{
	sndMod.callback = (MOD_CALLBACK)cmd->param32;
}



// --- MOD functions ---

static void MODStartTimer()
{
	u16 oldIME;

	oldIME = REG_IME;
	REG_IME = 0;

	TIMER0_CR = 0;
	TIMER0_DATA = sndVars.irqTimer;
	TIMER0_CR = TIMER_DIV_64 | TIMER_IRQ_REQ | TIMER_ENABLE;

	REG_IME = oldIME;
}

static void MODStopTimer()
{
	u16 oldIME;

	oldIME = REG_IME;
	REG_IME = 0;
	TIMER0_CR = 0;
	REG_IME = oldIME;
}

static void MODPlay(const void *modFile)
{
	s32 i;

	memset(&sndMod, 0, sizeof(sndMod));

	ModFileLoad(modFile, &sndMod, sndVars.memPool, sndVars.memPoolSize);
	sndMod.tick			= 0;
	sndMod.speed		= MOD_DEFAULT_SPEED;
	sndMod.state		= MOD_STATE_PLAY;

	for(i = 0; i < sndMod.channelCount; i++)
	{
		sndMod.channel[i].period = notePeriodTable[2*12];	// Middle C
		sndMod.channel[i].vol = 64;							// Full volume

		if((i & 3) == 0 || (i & 3) == 3)	// Panning levels go in 
			sndMod.channel[i].pan = 32;		// a down-up-up-down pattern
		else
			sndMod.channel[i].pan = 192;
	}

		// Set the order/row/rowPtr
	MODSeek(0, 0);
		// Set to default
	MODSetTempo(MOD_DEFAULT_TEMPO);

}	// MODPlay

static void MODStop()
{
	s32 i;

	MODStopTimer();

	for(i = 0; i < sndMod.channelCount; i++)
	{
		if (!(sndVars.channelBlocked & (1<<i)))
		{
			sndChannel[i].data = NULL;
			SCHANNEL_CR(i) = 0;
		}
	}

	sndMod.state = MOD_STATE_STOP;

}	// MODStop

// Returns FALSE if song ended, TRUE if still playing
static BOOL MODSeek(u32 order, u32 row)
{
	sndMod.curOrder = order;
	if(sndMod.curOrder >= sndMod.orderCount)
	{
		MODStop();			// Hit the end of the song, so stop it
		return FALSE;		// FALSE = song ended
	}
		
	sndMod.curRow = row;
	if(sndMod.curRow >= MOD_PATTERN_ROWS)
		sndMod.curRow = 0;	// Row was too high, just set it to 0

	sndMod.nextOrder = sndMod.curOrder + 1;
	sndMod.breakRow  = 0;
	sndMod.rowPtr    = sndMod.pattern[sndMod.order[sndMod.curOrder]] + 
					   sndMod.curRow*4*sndMod.channelCount;  // 4 bytes/channel/row

	return TRUE;			// TRUE = continue playing

}	// MODSeek

void MODUpdate()
{
	if(sndMod.state != MOD_STATE_PLAY)
		return;

	if(++sndMod.tick >= sndMod.speed)
	{
		sndMod.tick = 0;

		if(sndMod.patDelay == 0)
		{
			if(sndMod.curRow++ >= MOD_PATTERN_ROWS)
			{
				if(MODSeek(sndMod.nextOrder, sndMod.breakRow) == FALSE)
					return;		// FALSE = song ended

				sndMod.curRow++;
			}

			MODProcessRow();
		}
		else
		{
			sndMod.patDelay--;
		}
	}
	else
	{
		MODUpdateEffects();
	}

}	// MODUpdate

static void MODProcessRow()
{
	s32 curChannel;

	for(curChannel = 0; curChannel < sndMod.channelCount; curChannel++)
	{
		MOD_UPDATE_VARS vars = modDefaultVars[MOD_EFFECT_TABLE_ROW];

		vars.modChn = &sndMod.channel[curChannel];
		vars.sndChn = &sndChannel[curChannel];
		vars.chnIdx = curChannel;

			// Read in the pattern data, advancing rowPtr to the next channel in the process
		vars.note   = *sndMod.rowPtr++;
		vars.sample = *sndMod.rowPtr++;
		vars.effect = *sndMod.rowPtr++;
		vars.param  = *sndMod.rowPtr++;

			// Set these for the mid-ticks
		vars.modChn->note   = vars.note;
		vars.modChn->effect = vars.effect;
		vars.modChn->param  = vars.param;

			// Set sample and volume BEFORE effect processing, because some 
			// effects read the sample from the MOD channel rather than vars, 
			// and others need to override the default volume
		if(vars.sample != MOD_NO_SAMPLE)   // Never set local to memory anymore (explained below)
		{
				// Set sample memory
			vars.modChn->sample			= vars.sample;
			vars.modChn->vol			= sndMod.sample[vars.sample].vol;
			vars.modChn->finetune		= sndMod.sample[vars.sample].finetune;
				// Don't set mixer channel volume until after effect processing
				//vars.sndChn->vol          = vars.modChn->vol;
			vars.updateFlags			|= MOD_UPD_FLG_SET_VOL;
		}

			// Effect 0 is arpeggio, but is also used as no-effect if the param is 0 too
		if((vars.effect|vars.param) != 0 && modEffectTable[MOD_EFFECT_TABLE_ROW][vars.effect] != NULL)
		{
			(*modEffectTable[MOD_EFFECT_TABLE_ROW][vars.effect])(&vars);
		}

		if( !(sndVars.channelBlocked & (1<<curChannel)) )
			MODHandleUpdateFlags(&vars);
	}

}	// MODProcessRow

static void MODUpdateEffects()
{
	s32 curChannel;

	for(curChannel = 0; curChannel < sndMod.channelCount; curChannel++)
	{
		if((sndMod.channel[curChannel].effect|sndMod.channel[curChannel].param) != 0)
		{
			MOD_UPDATE_VARS vars = modDefaultVars[MOD_EFFECT_TABLE_MID];

			vars.modChn = &sndMod.channel[curChannel];
			vars.sndChn = &sndChannel[curChannel];
			vars.chnIdx = curChannel;

				// Already made sure there was an effect, so just check function
			if(modEffectTable[MOD_EFFECT_TABLE_MID][vars.modChn->effect] != NULL)
				(*modEffectTable[MOD_EFFECT_TABLE_MID][vars.modChn->effect])(&vars);

			if( !(sndVars.channelBlocked & (1<<curChannel)) )
				MODHandleUpdateFlags(&vars);
		}
	}

}	// MODUpdateEffects

static void MODHandleUpdateFlags(MOD_UPDATE_VARS *vars)
{
	u32 schnEnable = 0;

	if(vars->updateFlags & MOD_UPD_FLG_SET_VOL)
	{
			// Temporary volume to apply tremolo if needed
		u32 vol = vars->modChn->vol;
		if(vars->tremoloSlide != 0)
			vol = MODVolumeSlide(vol, vars->tremoloSlide);
		vars->sndChn->vol = vol;
		vars->sndChn->pan = vars->modChn->pan >> 1;
	}

	if( (vars->note != MOD_NO_NOTE) && 
		(vars->updateFlags & MOD_UPD_FLG_PLAY_NOTE) )
	{
		MODPlayNote(vars);

		schnEnable = SCHANNEL_ENABLE;
		SCHANNEL_CR(vars->chnIdx) = 0;
		SCHANNEL_SOURCE(vars->chnIdx) = (u32)vars->sndChn->data;
		if(vars->sndChn->loopLength == 0)
		{
			SCHANNEL_REPEAT_POINT(vars->chnIdx) = 0;
			SCHANNEL_LENGTH(vars->chnIdx) = vars->sndChn->length;
		}
		else
		{
			SCHANNEL_REPEAT_POINT(vars->chnIdx) = vars->sndChn->loopStart;
			SCHANNEL_LENGTH(vars->chnIdx) = vars->sndChn->loopLength;
		}
	}

	if(vars->fineSlide != 0)
	{
			// This has to happen after the note is played
		vars->modChn->period = 
			MODPitchSlide(vars->modChn->period, vars->fineSlide);
		vars->updateFlags |= MOD_UPD_FLG_SET_FREQ;
	}

	if(vars->updateFlags & MOD_UPD_FLG_SET_FREQ)
	{
		u32 period = vars->modChn->period;
			// Shift down 1 bit here because the slide range needs to be 
			// +-32 MOD periods, but the slide variable is +-64 because 
			// tremolo reuses the same code and needs the larger range
		if(vars->vibratoSlide != 0)
			period = MODPitchSlide(period, vars->vibratoSlide >> 1);

		vars->sndChn->timer = 65536 - (TIMER_VAL*period >> 17);
	}

	if(vars->sndChn->data == NULL)
	{
		SCHANNEL_CR(vars->chnIdx) = 0;
	}
	else
	{
		if(SCHANNEL_CR(vars->chnIdx) & SCHANNEL_ENABLE)
			schnEnable = SCHANNEL_ENABLE;

		SCHANNEL_TIMER(vars->chnIdx) = vars->sndChn->timer;
		SCHANNEL_CR(vars->chnIdx) = 
			SOUND_VOL(vars->sndChn->vol) | 
			SOUND_PAN(vars->sndChn->pan) | 
			(vars->sndChn->loopLength == 0 ? SOUND_ONE_SHOT : SOUND_REPEAT) | 
			SOUND_8BIT | 
			schnEnable;
	}

}	// MODHandleUpdateFlags

static void MODPlayNote(MOD_UPDATE_VARS *vars)
{
	const SAMPLE_HEADER *sample;
	u32 smpOffset;

		// Here's that special case that they didn't specify a sample before playing a note
	if(vars->modChn->sample == MOD_NO_SAMPLE)
	{
		return;
	}

		// Handy
	sample = &sndMod.sample[vars->modChn->sample];

		// 60 notes total, and one full set of notes for each finetune level
	vars->modChn->period = 
		MODGetNotePeriod(vars->modChn->note, vars->modChn->finetune);

		// Set up the mixer channel
	smpOffset = (vars->smpOffset << 8);

	if(sample->loopLength == 0)
	{
			// Note: length is in words, smpOffset is in samples (bytes).
		s32 length = (s32)sample->length - (smpOffset >> 2);
		if(length < 0)
			length = 0;	// Offset past the end, so don't play.

		vars->sndChn->length		= length;
		vars->sndChn->loopStart		= 0;
		vars->sndChn->loopLength	= 0;
	}
	else
	{
		s32 loopPoint = (s32)sample->loopStart - (smpOffset >> 2);
		if(loopPoint < 0)
		{
				// Offset was past the loop beginning. 
				// Play from the start of the loop.
			loopPoint = 0;
			smpOffset = (sample->loopStart << 2);
		}

		vars->sndChn->length		= loopPoint;
		vars->sndChn->loopStart		= loopPoint;
		vars->sndChn->loopLength	= sample->loopLength;
	}

	vars->sndChn->data = sample->smpData + smpOffset;

	vars->updateFlags |= MOD_UPD_FLG_SET_FREQ;

}	// MODPlayNote

static void MODSetTempo(u32 tempo)
{
	sndMod.tempo = tempo;
		// 1<<25=ARM7 freq, clock divided by 64, / tempo*2/5
	sndVars.irqTimer = 65536 - ((1<<25>>6>>1)*5 / tempo);

	if(sndMod.state == MOD_STATE_PLAY)
		MODStartTimer();

}	// MODSetTempo

static u32 MODGetNotePeriod(u32 note, u32 finetune)
{
	if(finetune == 0)
	{
			// Faster for the common case
		return notePeriodTable[note];
	}
	else
	{
		u32 low, high;

		if(finetune < 8)
		{
			low = notePeriodTable[note];
				// If adding 1 will run past the end of the table, 
				// calculate off the octave below
			if(note == 12*5 - 1)
				high = notePeriodTable[note - 11] >> 1;
			else
				high = notePeriodTable[note + 1];
		}
		else
		{
				// Get finetune to 0-7 for interpolation
			finetune -= 8;
				// If subtracting 1 will run below 0, 
				// calculate off the octave above
			if(note == 0)
				low = notePeriodTable[note + 11] << 1;
			else
				low = notePeriodTable[note - 1];
			high = notePeriodTable[note];
		}

			// Interpolate
		return low - ((low - high) * finetune >> 3);
	}

}	// MODGetPeriod



// --- Effect helper functions ---

// Returns volume after sliding up/down and clamping to (0,64)
static u32 MODVolumeSlide(u32 volume, s32 slide)
{
	volume += slide;
	if(volume > 64)			// Since volume is unsigned, this covers 
	{						// sliding too far in either direction
		if(slide > 0)
			volume = 64;	// Must have gone above 64
		else
			volume = 0;		// Must have gone below 0 and wrapped
	}

	return volume;
}

// Returns period after sliding up/down and 
// clamping to (MOD_PERIOD_MIN,MOD_PERRIOD_MAX)
static s32 MODPitchSlide(s32 period, s32 slide)
{
	period += slide;
	if (period > MOD_PERIOD_MAX)
		period = MOD_PERIOD_MAX;
	else if (period < MOD_PERIOD_MIN)
		period = MOD_PERIOD_MIN;

	return period;
}

static void MODInitVibrato(MOD_UPDATE_VARS *vars, MOD_VIBRATO_PARAMS *vibrato)
{
	if((vars->param & 0xf) != 0)
		vibrato->depth = vars->param & 0xf;
	if((vars->param >> 4) != 0)
		vibrato->speed = vars->param >> 4;
	if(vibrato->noRetrig == FALSE && vars->note != MOD_NO_NOTE)
		vibrato->tick = 0;
}

// Updates vibrato/tremolo, setting vibrato->slide to the range (-64, 63)
static s8 MODUpdateVibrato(MOD_VIBRATO_PARAMS *vibrato)
{
	s8 slide = 0;

		// Increment the tick. All wave types use a cycle of 0-63 on 
		// the tick. Since it's a 6-bit bitfield, it wraps automatically.
	vibrato->tick += vibrato->speed;
	//vibrato->tick &= 63;

	switch(vibrato->waveform)
	{
	case MOD_WAVEFORM_SINE:
			// Sine table is 0-255, and depth is 0-15. Shift 6 places off, 
			// giving a range of 0-64 (actually slightly less), and add or 
			// subtract it to get the final range +-64
		slide = (((s32)vibratoSineTab[vibrato->tick & 31])*vibrato->depth) >> 6;
		if(vibrato->tick >= 32)		// First 32 ticks are the 
			slide = -slide;			// positive half of the sine 
		break;						// wave, next 32 are negative

	case MOD_WAVEFORM_RAMP:
			// This is a linear ramp down, 64 ticks long. Subtract tick from 
			// 32 to give a value of 32 at tick 0, and -31 at tick 63 (just 
			// before wrapping), effectively creating a downward slide.
			// Divide by 8 for final range of +-64
		slide = (32 - vibrato->tick)*vibrato->depth >> 3;
		break;

	case MOD_WAVEFORM_SQUARE:
			// Square wave. Alternate between higest and lowest values. 
			// Since the final range needs to be +-64 and the depth ranges 
			// 0-15, just add or subtract 4x the depth
		slide = vibrato->depth << 2;
		if(vibrato->tick >= 32)
			slide = -slide;
		break;

	case MOD_WAVEFORM_RANDOM:
			// Since this table only ranges -128 to +127, shift down 
			// one less than sine to give the same final range
		slide = vibratoRandomTab[vibrato->tick]*vibrato->depth >> 5;
		break;
	}

	return slide;

}	// MODUpdateVibrato



// --- MOD effect functions ---

static void MODFXSetVol(MOD_UPDATE_VARS *vars)
{
	vars->modChn->vol = vars->param;
	if(vars->modChn->vol > 64)
		vars->modChn->vol = 64;
	vars->updateFlags |= MOD_UPD_FLG_SET_VOL;
}

static void MODFXSpeed(MOD_UPDATE_VARS *vars)
{
	if(vars->param < 32)				// 0-31 = set speed
		sndMod.speed = vars->param;
	else								// 32-255 = set tempo
		MODSetTempo(vars->param);
}

static void MODFXPan(MOD_UPDATE_VARS *vars)
{
	vars->modChn->pan = vars->param;
}

static void MODFXSampleOffset(MOD_UPDATE_VARS *vars)
{
	vars->smpOffset = vars->param;
}

static void MODFXJumpToOrder(MOD_UPDATE_VARS *vars)
{
	sndMod.nextOrder = vars->param;
	sndMod.curRow = MOD_PATTERN_ROWS;	// Break next update
}

static void MODFXBreakToRow(MOD_UPDATE_VARS *vars)
{
	sndMod.breakRow = vars->param;
	sndMod.curRow = MOD_PATTERN_ROWS;	// Break next update
}

static void MODFXArpeggioRow(MOD_UPDATE_VARS *vars)
{
	vars->modChn->arpeggioTick = 0;
		// Cancel the effect if no note has been played on this channel
	if(vars->note == MOD_NO_NOTE)
		vars->modChn->effect = vars->modChn->param = 0;
}

static void MODFXArpeggioMid(MOD_UPDATE_VARS *vars)
{
	u32 arpNote = 0;

	if(++vars->modChn->arpeggioTick > 2)
		vars->modChn->arpeggioTick = 0;

	switch(vars->modChn->arpeggioTick)
	{
		case 0: arpNote = vars->modChn->note;								break;
		case 1: arpNote = vars->modChn->note + (vars->modChn->param >> 4);	break;
		case 2: arpNote = vars->modChn->note + (vars->modChn->param & 0xf);	break;
	}

	if(arpNote > MOD_MAX_NOTE)
		arpNote = MOD_MAX_NOTE;

	vars->modChn->period = MODGetNotePeriod(arpNote, vars->modChn->finetune);

	vars->updateFlags |= MOD_UPD_FLG_SET_FREQ;

}	// MODFXArpeggioMid

static void MODFXVolslideRow(MOD_UPDATE_VARS *vars)
{
	if(vars->param != 0)
	{
		if((vars->param & 0x0f) == 0)
			vars->modChn->volslideSpeed = vars->param >> 4;
		else if((vars->param & 0xf0) == 0)
			vars->modChn->volslideSpeed = -vars->param;
		else
			vars->modChn->effect = vars->modChn->param = 0;
	}
}

static void MODFXVolslideMid(MOD_UPDATE_VARS *vars)
{
	vars->modChn->vol = 
		MODVolumeSlide(vars->modChn->vol, vars->modChn->volslideSpeed);
	vars->updateFlags |= MOD_UPD_FLG_SET_VOL;
}

static void MODFXPortaRow(MOD_UPDATE_VARS *vars)
{
	if(vars->param != 0)
		vars->modChn->portaSpeed = vars->param;
}

static void MODFXPortaDownMid(MOD_UPDATE_VARS *vars)
{
	vars->modChn->period = 
		MODPitchSlide(vars->modChn->period, vars->modChn->portaSpeed);

	vars->updateFlags |= MOD_UPD_FLG_SET_FREQ;
}

static void MODFXPortaUpMid(MOD_UPDATE_VARS *vars)
{
	vars->modChn->period = 
		MODPitchSlide(vars->modChn->period, -vars->modChn->portaSpeed);

	vars->updateFlags |= MOD_UPD_FLG_SET_FREQ;
}

static void MODFXTonePortaRow(MOD_UPDATE_VARS *vars)
{
	if(vars->modChn->sample != MOD_NO_SAMPLE)
	{
		if(vars->note != MOD_NO_NOTE)
			vars->modChn->tonePortaNote = vars->note;
		// Else use the note like it is

		if(vars->param != 0)
			vars->modChn->tonePortaSpeed = vars->param;
		// Else use the speed like it is

			// Don't play the note or we wouldn't have anything to slide to
		vars->updateFlags &= ~MOD_UPD_FLG_PLAY_NOTE;
	}
	else	// No sample. Can't read finetune on mid-ticks, so cancel the effect
	{
		vars->modChn->effect = vars->modChn->param = 0;
	}
}

static void MODFXTonePortaMid(MOD_UPDATE_VARS *vars)
{
	u32 tonePortaDestFreq;

		// Fetch the frequency of the target note 
	tonePortaDestFreq = 
		MODGetNotePeriod(vars->modChn->tonePortaNote, vars->modChn->finetune);

	if(vars->modChn->period < tonePortaDestFreq)
	{
			// Slide down toward note
		vars->modChn->period = MODPitchSlide(
			vars->modChn->period, vars->modChn->tonePortaSpeed);
		if(vars->modChn->period > tonePortaDestFreq)
		{
				// Slid too far, back up to the destination
			vars->modChn->period = tonePortaDestFreq;
		}
	}
	else if(vars->modChn->period > tonePortaDestFreq)
	{
			// Slide up toward note
		vars->modChn->period = MODPitchSlide(
			vars->modChn->period, -vars->modChn->tonePortaSpeed);
		if(vars->modChn->period < tonePortaDestFreq)
		{
				// Slid too far, back down to the destination
			vars->modChn->period = tonePortaDestFreq;
		}
	}
	// Else we're already at the target frequency

		// Mix channel increment will need recalculated
	vars->updateFlags |= MOD_UPD_FLG_SET_FREQ;

}	// MODFXTonePortaMid

static void MODFXVibratoRow(MOD_UPDATE_VARS *vars)
{
	MODInitVibrato(vars, &vars->modChn->vibrato);
}

static void MODFXVibratoMid(MOD_UPDATE_VARS *vars)
{
	vars->vibratoSlide = MODUpdateVibrato(&vars->modChn->vibrato);
	vars->updateFlags |= MOD_UPD_FLG_SET_FREQ;
}

static void MODFXTremoloRow(MOD_UPDATE_VARS *vars)
{
	MODInitVibrato(vars, &vars->modChn->tremolo);
}

static void MODFXTremoloMid(MOD_UPDATE_VARS *vars)
{
	vars->tremoloSlide = MODUpdateVibrato(&vars->modChn->tremolo);
	vars->updateFlags |= MOD_UPD_FLG_SET_VOL;
}

static void MODFXVSldTPortaRow(MOD_UPDATE_VARS *vars)
{
		// Param goes with the volslide part, tone porta just continues
		// So handle volslide like normal
	MODFXVolslideRow(vars);
		// Now trick the tone porta into thinking there was a 
		// 0 'continue' param. This local param won't be used again, 
		// so changing it won't hurt anything
	vars->param = 0;
	MODFXTonePortaRow(vars);
}

static void MODFXVSldTPortaMid(MOD_UPDATE_VARS *vars)
{
	MODFXVolslideMid(vars);
	MODFXTonePortaMid(vars);
}

static void MODFXVSldVibratoRow(MOD_UPDATE_VARS *vars)
{
		// Param goes with the volslide part, tone porta just continues
		// So handle volslide like normal
	MODFXVolslideRow(vars);
		// Now trick the vibrato into thinking there was a 
		// 0 'continue' param. This local param won't be used again, 
		// so changing it won't hurt anything
	vars->param = 0;
	MODFXVibratoRow(vars);
}

static void MODFXVSldVibratoMid(MOD_UPDATE_VARS *vars)
{
	MODFXVolslideMid(vars);
	MODFXVibratoMid(vars);
}

static void MODFXSpecialRow(MOD_UPDATE_VARS *vars)
{
		// Since the upper 4 bits of the parameter are used as the effect 
		// type, this saves us having to extract the lower bits every time
	u32 param = vars->modChn->param & 0xf;

	switch(vars->modChn->param >> 4)
	{
	case 0x0:		// Undefined, we use it for callback
		if(sndMod.callback != NULL)
			sndMod.callback(param, TRUE);
		break;
	case 0x1:		// Fine porta up
		vars->fineSlide = param;
		vars->updateFlags |= MOD_UPD_FLG_SET_FREQ;
		break;
	case 0x2:		// Fine porta down
		vars->fineSlide = -param;
		vars->updateFlags |= MOD_UPD_FLG_SET_FREQ;
		break;
	case 0x3:		// Glissando (unsupported)
		break;
	case 0x4:		// Vibrato waveform
		vars->modChn->vibrato.waveform =  param & 3;
		vars->modChn->vibrato.noRetrig = (param & 4) ? TRUE : FALSE;
		break;
	case 0x5:		// Set finetune
		vars->modChn->finetune = param;
		if (vars->modChn->note != MOD_NO_NOTE)
		{
			vars->modChn->period = 
				MODGetNotePeriod(vars->modChn->note, vars->modChn->finetune);
			vars->updateFlags |= MOD_UPD_FLG_SET_FREQ;
		}
		break;
	case 0x6:		// Pattern loop
		if(param == 0)
			vars->modChn->patLoopPos = sndMod.curRow;
		else
		{
			if(vars->modChn->patLoopCount == 0)
				vars->modChn->patLoopCount = param+1;

			if(--vars->modChn->patLoopCount != 0)
			{		// Loop back to the stored row in this order next tick
				sndMod.breakRow  = vars->modChn->patLoopPos;
				sndMod.nextOrder = sndMod.curOrder;		// Don't advance the order
				sndMod.curRow    = MOD_PATTERN_ROWS;	// This triggers the break
			}
		}
		break;
	case 0x7:		// Tremolo waveform
		vars->modChn->tremolo.waveform =  param & 3;
		vars->modChn->tremolo.noRetrig = (param & 4) ? TRUE : FALSE;
		break;
	case 0x8:		// Set panning
		vars->modChn->pan = (param << 4) | param;
		break;
	case 0x9:		// Retrigger note
		vars->modChn->retrigTick = param;
		break;
	case 0xA:		// Fine volume slide up
		vars->modChn->vol = MODVolumeSlide(vars->modChn->vol, param);
		vars->updateFlags |= MOD_UPD_FLG_SET_VOL;
		break;
	case 0xB:		// Fine volume slide down
		vars->modChn->vol = MODVolumeSlide(vars->modChn->vol, -param);
		vars->updateFlags |= MOD_UPD_FLG_SET_VOL;
		break;
	case 0xC:		// Note cut
		vars->modChn->noteCutTick = param;
		break;
	case 0xD:		// Note delay
		vars->modChn->noteDelayTick = param;
			// Don't set the mixer channel vol until the note plays
		vars->updateFlags &= ~(MOD_UPD_FLG_PLAY_NOTE | MOD_UPD_FLG_SET_VOL);
		break;
	case 0xE:		// Pattern delay
		sndMod.patDelay = param;
		break;
	case 0xF:		// Undefined (think of your own use for it!)
		break;
	}

}	// MODFXSpecialRow

static void MODFXSpecialMid(MOD_UPDATE_VARS *vars)
{
	switch(vars->modChn->param >> 4)
	{
	case 0x0:		// Undefined, we use it for callback
		if(sndMod.callback != NULL)
			sndMod.callback(vars->modChn->param & 0xf, FALSE);
		break;
	case 0x1:		// Fine porta up
		break;
	case 0x2:		// Fine porta down
		break;
	case 0x3:		// Glissando
		break;
	case 0x4:		// Vibrato waveform
		break;
	case 0x5:		// Set finetune
		break;
	case 0x6:		// Pattern loop
		break;
	case 0x7:		// Tremolo waveform
		break;
	case 0x8:		// Set panning (unsupported)
		break;
	case 0x9:		// Retrigger note
		if(--vars->modChn->retrigTick == 0)
		{
			vars->note = vars->modChn->note;
			vars->updateFlags |= MOD_UPD_FLG_PLAY_NOTE;
		}
		break;
	case 0xA:		// Fine volume slide up
		break;
	case 0xB:		// Fine volume slide down
		break;
	case 0xC:		// Note cut
		if(--vars->modChn->noteCutTick == 0)
		{
			vars->modChn->vol = 0;
			vars->updateFlags |= MOD_UPD_FLG_SET_VOL;
			vars->modChn->effect = vars->modChn->param = 0;
		}
		break;
	case 0xD:		// Note delay
		if(--vars->modChn->noteDelayTick == 0)
		{
			vars->note = vars->modChn->note;
			vars->updateFlags |= MOD_UPD_FLG_PLAY_NOTE | MOD_UPD_FLG_SET_VOL;
			vars->modChn->effect = vars->modChn->param = 0;
		}
		break;
	case 0xE:		// Pattern delay
		break;
	case 0xF:		// Undefined (think of your own use for it!)
		break;
	}

}	// MODFXSpecialMid

	//Non - MOD function
//Activate a channel with all parameters if already activate do it to update status
static void FlashChannel(u8 channel)
{
  if(sndChannel[channel].loopLength == 0)//one shot
  {
	switch(sndChannel[channel].format)
	{
	case 0 : // 8 BITS
		SCHANNEL_CR(channel) = SCHANNEL_ENABLE | SOUND_ONE_SHOT | SOUND_VOL(sndChannel[channel].vol) | SOUND_PAN(sndChannel[channel].pan) | SOUND_FORMAT_8BIT;
		break;
	case 1 : // 16 BITS
		SCHANNEL_CR(channel) = SCHANNEL_ENABLE | SOUND_ONE_SHOT | SOUND_VOL(sndChannel[channel].vol) | SOUND_PAN(sndChannel[channel].pan) | SOUND_FORMAT_16BIT;
		break;
	case 2 : // ADPCM
		SCHANNEL_CR(channel) = SCHANNEL_ENABLE | SOUND_ONE_SHOT | SOUND_VOL(sndChannel[channel].vol) | SOUND_PAN(sndChannel[channel].pan) | SOUND_FORMAT_ADPCM;
		break;
	case 3 : // PSG
		SCHANNEL_CR(channel) = SCHANNEL_ENABLE | SOUND_ONE_SHOT | SOUND_VOL(sndChannel[channel].vol) | SOUND_PAN(sndChannel[channel].pan) | SOUND_FORMAT_PSG;
		break;
	}
  }
  else
  {
	switch(sndChannel[channel].format)
	{
	case 0 : // 8 BITS
		SCHANNEL_CR(channel) = SCHANNEL_ENABLE | SOUND_REPEAT | SOUND_VOL(sndChannel[channel].vol) | SOUND_PAN(sndChannel[channel].pan) | SOUND_FORMAT_8BIT;
		break;
	case 1 : // 16 BITS
		SCHANNEL_CR(channel) = SCHANNEL_ENABLE | SOUND_REPEAT | SOUND_VOL(sndChannel[channel].vol) | SOUND_PAN(sndChannel[channel].pan) | SOUND_FORMAT_16BIT;
		break;
	case 2 : // ADPCM
		SCHANNEL_CR(channel) = SCHANNEL_ENABLE | SOUND_REPEAT | SOUND_VOL(sndChannel[channel].vol) | SOUND_PAN(sndChannel[channel].pan) | SOUND_FORMAT_ADPCM;
		break;
	case 3 : // PSG
		SCHANNEL_CR(channel) = SCHANNEL_ENABLE | SOUND_REPEAT | SOUND_VOL(sndChannel[channel].vol) | SOUND_PAN(sndChannel[channel].pan) | SOUND_FORMAT_PSG;
		break;
	}
  }
}

static void SndChgRepeat(SND_COMMAND *cmd)
{
  //change volume and reflash SCHANNEL_CR with all parameters (don't work if not)
  sndChannel[cmd->param[0]].loopLength = cmd->param[1];
  SCHANNEL_REPEAT_POINT(cmd->param[0]) = (u16)cmd->param32;
  FlashChannel(cmd->param[0]);
}

static void SndChgLength(SND_COMMAND *cmd)
{
  //change volume and reflash SCHANNEL_CR with all parameters (don't work if not)
  SCHANNEL_LENGTH(cmd->param[0]) = cmd->param32;
  FlashChannel(cmd->param[0]);
}

static void SndChgVolume(SND_COMMAND *cmd)
{
  //change volume and reflash SCHANNEL_CR with all parameters (don't work if not)
  sndChannel[cmd->param[0]].vol = cmd->param[1];
  FlashChannel(cmd->param[0]);
}

static void SndChgPan(SND_COMMAND *cmd)
{
  //change volume and reflash SCHANNEL_CR with all parameters (don't work if not)
  sndChannel[cmd->param[0]].pan = cmd->param[1];
  FlashChannel(cmd->param[0]);
}

static void SndChgTimer(SND_COMMAND *cmd)
{
  //change volume and reflash SCHANNEL_CR with all parameters (don't work if not)
  sndChannel[cmd->param[0]].timer = (int)(cmd->param32);
  SCHANNEL_TIMER(cmd->param[0])  = SOUND_FREQ(sndChannel[cmd->param[0]].timer);
}

//Start a song
static void SngCmdPlay(SND_COMMAND *cmd)
{
  TransferSound *snd = IPC->soundData;
  IPC->soundData = 0;

  sndChannel[cmd->param[0]].timer = ((int)(snd->data[cmd->param[0]].rate));
  sndChannel[cmd->param[0]].vol = snd->data[cmd->param[0]].vol;
  sndChannel[cmd->param[0]].pan = snd->data[cmd->param[0]].pan;
  sndChannel[cmd->param[0]].format = snd->data[cmd->param[0]].format;
  SCHANNEL_TIMER(cmd->param[0])  = SOUND_FREQ(sndChannel[cmd->param[0]].timer);
  SCHANNEL_SOURCE(cmd->param[0]) = (uint32)(snd->data[cmd->param[0]].data);
  SCHANNEL_LENGTH(cmd->param[0]) = (snd->data[cmd->param[0]].len) >> 2;
  if(cmd->param[1] == 0)//one shot
  {
	sndChannel[cmd->param[0]].loopLength = 0;
	SCHANNEL_REPEAT_POINT(cmd->param[0]) = 0;
  }
  else
  {
	sndChannel[cmd->param[0]].loopLength = ((snd->data[cmd->param[0]].len) >> 2);
	SCHANNEL_REPEAT_POINT(cmd->param[0]) = (u16)cmd->param32;
  }
  FlashChannel(cmd->param[0]);
}

//stop a song
static void SngCmdStop(SND_COMMAND *cmd)
{
	//Desactivate Channel
	SCHANNEL_CR(cmd->param[0]) = 0; 
}

//pause a song 0 for unpause 1 for pause
static void SngCmdPause(SND_COMMAND *cmd)
{
	//Frequency = 0 => bits are read at 0hz => pause read
	if(cmd->param[1] == 1)
		SCHANNEL_TIMER(cmd->param[0]) = 0;
	else
		SCHANNEL_TIMER(cmd->param[0])  = SOUND_FREQ(sndChannel[cmd->param[0]].timer);
}

	//Mic functions
//start recording
/*
static void MicCmdStart(SND_COMMAND *cmd)
{
	PM_SetAmp(PM_AMP_ON);
	StartRecording((u8*)(cmd->param32) ,((int)(cmd->param[0]))*1600);
}

//stop recording
static void MicCmdStop(SND_COMMAND *cmd)
{
	StopRecording();
}*/
