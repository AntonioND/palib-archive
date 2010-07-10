#define PA_NO_DEPRECATION // we need access to the legacy PA_IPC structure
#include <PA9.h>

void PA_PlayPSG(u8 chan, u8 vol, u8 pan, u32 freq, u8 duty) {
	PA_FifoMsg msg;
	msg.type = PA_MSG_PSG;
	msg.PSGMsg.freq = freq << 1; // a bug that fixes a bug.
	msg.PSGMsg.chan = (chan & 7) | 8; // clip it to a valid PSG channel
	msg.PSGMsg.vol = vol & 0x7F;
	msg.PSGMsg.pan = pan & 0x7F;
	msg.PSGMsg.duty = duty & 7;
	PA_SendFifoMsg(msg);
}

void PA_PlaySoundEx2(u8 PA_Channel, const void* data, s32 length, u8 volume, int freq, s16 format, bool repeat, int repeatPoint) {
	// Flush the data
	DC_FlushRange(data, length);
	PA_IPC.Sound[PA_Channel].Busy = 1; // now busy
	// keep this in for compatibility with some functions
	// Sound commands
	//PA_IPC.Sound[PA_Channel].Command |= (1<<PAIPC_PLAY); // play
	PA_IPC.Sound[PA_Channel].Data = (u32*)data;
	PA_IPC.Sound[PA_Channel].Volume = volume;
	PA_IPC.Sound[PA_Channel].Pan = 64;
	PA_IPC.Sound[PA_Channel].Rate = freq;
	PA_IPC.Sound[PA_Channel].Length = length - 4;
	PA_IPC.Sound[PA_Channel].Format = format;
	PA_IPC.Sound[PA_Channel].Repeat = repeat;
	PA_IPC.Sound[PA_Channel].RepeatPoint = repeatPoint;
	PA_IPC.Sound[PA_Channel].Duty = 0;
//	PA_IPC.Sound[PA_Channel].Pause = 0;
	PA_IPC.Sound[PA_Channel].Command |= (1 << PAIPC_PLAY); // play
}

void PA_SoundStop(u8 PA_Channel) {
	PA_IPC.Sound[PA_Channel].Command |= (1 << PAIPC_STOP);
}
