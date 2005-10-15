// ----- Change log -----
// 05/09/05 - Created.
// ----------------------

#include <nds.h>
#include <string.h>
#include "Sound9.h"
#include "../../SoundCommon.h"


// ----- Global functions -----

	// Call once on startup
void SndInit9()
{
	memset(sndControl, 0, sizeof(SND_CONTROL));
	sndControl->bInitialized = TRUE;
}

	// Send command to ARM7 to set the memory area to load MOD files into 
	// for playing. Must be called at least once at startup, but can be 
	// called repeatedly to change the size of the sound area as needed.
	//
	// Args:
	//	memPool			Pointer to an area in main memory.
	//	memPoolSize		Size of the area. Must be at least the size 
	//					of your largest MOD file.
void SndSetMemPool(void *memPool, u32 memPoolSize)
{
	SND_COMMAND *cmd = &sndControl->cmd[sndControl->curCmd];

	memset(cmd, 0, sizeof(SND_COMMAND));

	ASSERT(memPool != NULL);
	ASSERT(size < (1<<24));	// Only 3 bytes available in parameters

	cmd->cmdType  = SND_CMD_SETMEMPOOL;
	cmd->param[0] = (u8)(memPoolSize);
	cmd->param[1] = (u8)(memPoolSize >> 8);
	cmd->param[2] = (u8)(memPoolSize >> 16);
	cmd->param32  = (u32)memPool;

	sndControl->curCmd++;
	sndControl->curCmd &= MAX_SND_COMMANDS-1;
}

	// Send command to ARM7 to play a song.
	//
	// Args:
	//	modFile		Pointer to a standard .mod file.
void SndPlayMOD(void *modFile)
{
	SND_COMMAND *cmd = &sndControl->cmd[sndControl->curCmd];

	memset(cmd, 0, sizeof(SND_COMMAND));

	cmd->cmdType = SND_CMD_PLAYSONG;
	cmd->param32 = (u32)modFile;

	sndControl->curCmd++;
	sndControl->curCmd &= MAX_SND_COMMANDS-1;
}

	// Send command to ARM7 to stop the song.
void SndStopMOD()
{
	SND_COMMAND *cmd = &sndControl->cmd[sndControl->curCmd];

	memset(cmd, 0, sizeof(SND_COMMAND));

	cmd->cmdType = SND_CMD_STOPSONG;

	sndControl->curCmd++;
	sndControl->curCmd &= MAX_SND_COMMANDS-1;
}

	// Send command to ARM7 to pause or unpause the song.
void SndPauseMOD(BOOL bPaused)
{
	SND_COMMAND *cmd = &sndControl->cmd[sndControl->curCmd];

	memset(cmd, 0, sizeof(SND_COMMAND));

	cmd->cmdType = SND_CMD_STOPSONG;
	cmd->param[0] = (u8)bPaused;

	sndControl->curCmd++;
	sndControl->curCmd &= MAX_SND_COMMANDS-1;
}

	// Send command to ARM7 to set the MOD callback function.
	// Callback is triggered by E0x effects in the song.
void SndSetMODCallback(MOD_CALLBACK callback)
{
	SND_COMMAND *cmd = &sndControl->cmd[sndControl->curCmd];

	memset(cmd, 0, sizeof(SND_COMMAND));

	cmd->cmdType = SND_CMD_SETCALLBACK;
	cmd->param32 = (u32)callback;

	sndControl->curCmd++;
	sndControl->curCmd &= MAX_SND_COMMANDS-1;
}
