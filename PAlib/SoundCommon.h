// ----- Change log -----
// 05/08/05 - Created
// ----------------------

#ifndef SOUNDCOMMON_H
#define SOUNDCOMMON_H

#include <nds.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ASSERT(arg)

// ----- Standard stuff that should be in NDSLIB but isn't -----

#ifndef NULL
#define NULL ((void*)0)
#endif

#ifndef BOOL
typedef u8 BOOL;
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


// ----- Constants -----

#define MAX_SND_COMMANDS	32

typedef void (*MOD_CALLBACK)(u32 param, BOOL bRowTick);

typedef enum _SND_COMMAND_TYPE
{
	SND_CHG_VOLUME,			//Param[0]:Channel Param[1]: Volume
	SND_CHG_PAN,			//Param[0]:Channel Param[1]: Pan
	SND_CHG_TIMER,			//Param[0]:Channel Param32 : Timer
	SND_CHG_REPEAT,			//Param[0]:Channel Param[1] : Repeat true/false Param32 : Repeat Point
	SND_CHG_LENGTH,			//Param[0]:Channel Param32 : Length
	SNG_CMD_PLAY,			//Param[0]:Channel Param[1]: 0=OneShot, 1=Repeat -> Param32 = Repeat Point
	SNG_CMD_STOP,			//Param[0]:Channel
	SNG_CMD_PAUSE,			//Param[0]:Channel Param[1]: 1=pause, 0=unpause
	SND_CMD_SETMEMPOOL,		// Param32: Address, Param[0 to 2]: Size
	SND_CMD_PLAYSONG,		// Param32: Pointer to song file (standard .mod)
	SND_CMD_STOPSONG,		// No params
	SND_CMD_PAUSESONG,		// Param[0]: 1=pause, 0=unpause
	SND_CMD_SETCALLBACK,	// Param32: MOD_CALLBACK function pointer
	MIC_CMD_START,			// Param[0]: Length/1600, Param32 : Address to s8* buffer
	MIC_CMD_STOP,			// No Params

	SND_CMD_NUM,

} SND_COMMAND_TYPE;


// ----- Structures -----

	// Commands filled out by ARM9 to tell ARM7 what to do
typedef struct _SND_COMMAND
{
	u8 cmdType;		// SND_COMMAND_TYPE enum
	u8 param[3];	// Depend on cmdType
	u32 param32;	// Mainly for commands that need to send a pointer

} SND_COMMAND;

	// Processor communication area
typedef struct _SND_CONTROL
{
	SND_COMMAND	cmd[MAX_SND_COMMANDS];
	u8			curCmd;
	BOOL		bInitialized;	// Set by ARM9's SndInit

} SND_CONTROL;


typedef struct _SOUND_CHANNEL
{
	const s8	*data;
	u32			length;
	u16			loopStart;
	u32			loopLength;
	int			timer;
	u8			vol;
	u8			pan;
	u8			format;

} SOUND_CHANNEL;

// ----- Global variables -----

	// Magic address, after IPC struct
#define sndControl		((SND_CONTROL*)((u32)(IPC) + sizeof(TransferRegion)))

// ----------

#ifdef __cplusplus
}
#endif

#endif	// SOUNDCOMMON_H
