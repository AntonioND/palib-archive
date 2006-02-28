// ----- Change log -----
// 05/09/05 - Created.
// ----------------------

#include <nds.h>
#include <string.h>
#include "Sound9.h"
#include "SoundCommon.h"

//Channels
SOUND_CHANNEL	SndChannel[SND_MAX_CHANNELS];

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


void SndPlay(u8 PA_Channel, const void* data, s32 length, u8 volume, s16 freq, s16 format, BOOL repeat, int repeatPoint)
{
	SND_COMMAND *cmd = &sndControl->cmd[sndControl->curCmd];

	memset(cmd, 0, sizeof(SND_COMMAND));

	cmd->cmdType = SNG_CMD_PLAY;
	cmd->param[0] = PA_Channel;
	cmd->param[1] = repeat;
	cmd->param32  = repeatPoint;

	sndControl->curCmd++;
	sndControl->curCmd &= MAX_SND_COMMANDS-1;
}

extern void SndStop(u8 PA_Channel)
{
	SND_COMMAND *cmd = &sndControl->cmd[sndControl->curCmd];

	memset(cmd, 0, sizeof(SND_COMMAND));

	cmd->cmdType = SNG_CMD_STOP;
	cmd->param[0] = PA_Channel;

	sndControl->curCmd++;
	sndControl->curCmd &= MAX_SND_COMMANDS-1;
}

extern void SndPause(u8 PA_Channel)
{
	SND_COMMAND *cmd = &sndControl->cmd[sndControl->curCmd];

	memset(cmd, 0, sizeof(SND_COMMAND));

	cmd->cmdType = SNG_CMD_PAUSE;
	cmd->param[0] = PA_Channel;
	cmd->param[1] = 1;

	sndControl->curCmd++;
	sndControl->curCmd &= MAX_SND_COMMANDS-1;
}


extern void SndUnpause(u8 PA_Channel)
{
	SND_COMMAND *cmd = &sndControl->cmd[sndControl->curCmd];

	memset(cmd, 0, sizeof(SND_COMMAND));

	cmd->cmdType = SNG_CMD_PAUSE;
	cmd->param[0] = PA_Channel;
	cmd->param[1] = 0;

	sndControl->curCmd++;
	sndControl->curCmd &= MAX_SND_COMMANDS-1;
}

extern void SndChgVolume(u8 PA_Channel, u8 Volume)
{
	SND_COMMAND *cmd = &sndControl->cmd[sndControl->curCmd];

	SndChannel[PA_Channel].vol = Volume;

	memset(cmd, 0, sizeof(SND_COMMAND));

	cmd->cmdType = SND_CHG_VOLUME;
	cmd->param[0] = PA_Channel;
	cmd->param[1] = Volume;

	sndControl->curCmd++;
	sndControl->curCmd &= MAX_SND_COMMANDS-1;
}

extern void SndChgPan(u8 PA_Channel, u8 Pan)
{
	SND_COMMAND *cmd = &sndControl->cmd[sndControl->curCmd];

	SndChannel[PA_Channel].pan = Pan;

	memset(cmd, 0, sizeof(SND_COMMAND));

	cmd->cmdType = SND_CHG_PAN;
	cmd->param[0] = PA_Channel;
	cmd->param[1] = Pan;

	sndControl->curCmd++;
	sndControl->curCmd &= MAX_SND_COMMANDS-1;
}

extern void SndChgTimer(u8 PA_Channel, int Timer)
{
	SND_COMMAND *cmd = &sndControl->cmd[sndControl->curCmd];

	SndChannel[PA_Channel].timer = Timer;

	memset(cmd, 0, sizeof(SND_COMMAND));

	cmd->cmdType = SND_CHG_TIMER;
	cmd->param[0] = PA_Channel;
	cmd->param32 = Timer;

	sndControl->curCmd++;
	sndControl->curCmd &= MAX_SND_COMMANDS-1;
}

void SndChgRepeat(u8 PA_Channel, BOOL repeat, u32 RepeatPoint)
{
	SND_COMMAND *cmd = &sndControl->cmd[sndControl->curCmd];

	SndChannel[PA_Channel].loopStart = RepeatPoint;
	
	memset(cmd, 0, sizeof(SND_COMMAND));

	cmd->cmdType = SND_CHG_REPEAT;
	cmd->param[0] = PA_Channel;
	cmd->param[1] = repeat;
	cmd->param32 = RepeatPoint;

	sndControl->curCmd++;
	sndControl->curCmd &= MAX_SND_COMMANDS-1;
}

void SndChgLength(u8 PA_Channel, u32 Length)
{
	SND_COMMAND *cmd = &sndControl->cmd[sndControl->curCmd];

	SndChannel[PA_Channel].loopLength = Length;

	memset(cmd, 0, sizeof(SND_COMMAND));

	cmd->cmdType = SND_CHG_LENGTH;
	cmd->param[0] = PA_Channel;
	cmd->param32 = Length;

	sndControl->curCmd++;
	sndControl->curCmd &= MAX_SND_COMMANDS-1;
}

void StartRecording(u8* buffer, int length)
{
	SND_COMMAND *cmd = &sndControl->cmd[sndControl->curCmd];

	memset(cmd, 0, sizeof(SND_COMMAND));

	cmd->cmdType = MIC_CMD_START;
	cmd->param[0] = (length/1600);
	cmd->param32 = (u32)buffer;

	sndControl->curCmd++;
	sndControl->curCmd &= MAX_SND_COMMANDS-1;
}

void StopRecording()
{
	SND_COMMAND *cmd = &sndControl->cmd[sndControl->curCmd];

	memset(cmd, 0, sizeof(SND_COMMAND));

	cmd->cmdType = MIC_CMD_STOP;

	sndControl->curCmd++;
	sndControl->curCmd &= MAX_SND_COMMANDS-1;
}
