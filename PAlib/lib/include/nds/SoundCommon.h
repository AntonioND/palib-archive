// ----- Change log -----
// 05/08/05 - Created
// ----------------------

#ifndef SOUNDCOMMON_H
#define SOUNDCOMMON_H

#include <nds.h>

#ifdef __cplusplus
extern "C" {
#endif



// ----- Standard stuff that should be in NDSLIB but isn't -----

#ifndef NULL
#define NULL ((void*)0)
#endif

#ifndef BOOLEAN
typedef u8 BOOLEAN;
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


// ----- Constants -----

#define MAX_SND_COMMANDS	32

typedef void (*MOD_CALLBACK)(u32 param, BOOLEAN bRowTick);

typedef enum _SND_COMMAND_TYPE
{
	SND_CMD_SETMEMPOOL,		// Param32: Address, Param[0 to 2]: Size
	SND_CMD_PLAYSONG,		// Param32: Pointer to song file (standard .mod)
	SND_CMD_STOPSONG,		// No params
	SND_CMD_PAUSESONG,		// Param[0]: 1=pause, 0=unpause
	SND_CMD_SETCALLBACK,	// Param32: MOD_CALLBACK function pointer
	SND_CMD_SETVOLUME,		// Param32: volume

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
	BOOLEAN		bInitialized;	// Set by ARM9's SndInit
	bool		toFade;

} SND_CONTROL;

// <MODPLAYER> IPC struct and constants for the modplayer
#define MT_MSG_INIT 0x4444
#define MT_MSG_END 0x5555
typedef struct {
	volatile u16 command;
	u8 *module;
    volatile u16 synchro_fx;
} MT_MSG_CMD, *LPMT_MSG_CMD;


// ----- Global variables -----

	// Magic address, after IPC struct
#define sndControl		((SND_CONTROL*)((u32)(IPC) + sizeof(TransferRegion)))

#define modControl		((MT_MSG_CMD*)((u32)(IPC) + sizeof(TransferRegion) + sizeof(SND_CONTROL)))
		
// ----------

#ifdef __cplusplus
}
#endif

#endif	// SOUNDCOMMON_H
