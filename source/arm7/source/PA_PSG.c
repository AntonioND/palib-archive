#include "PA_Internal.h"

void PA_FifoPSGMsg(PA_FifoMsg* msg){
	u32 ch = msg->PSGMsg.chan; // for faster access
	SCHANNEL_TIMER(ch) = TIMER_FREQ((s32) msg->PSGMsg.freq);
	SCHANNEL_CR(ch) = SCHANNEL_ENABLE | SOUND_VOL(msg->PSGMsg.vol) | SOUND_PAN(msg->PSGMsg.pan)
		| SOUND_FORMAT_PSG | (msg->PSGMsg.duty << 24);
}
