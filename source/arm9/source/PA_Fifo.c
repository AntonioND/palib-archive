#include <PA9.h>

void PA_FifoMsgHandler(int bytes, void* dummy);

void PA_InitFifo(){
	fifoSetDatamsgHandler(FIFO_PALIB, PA_FifoMsgHandler, 0);
}

void PA_FifoMsgHandler(int bytes, void* dummy){
	PA_FifoMsg msg;
	PA_GetFifoMsg(msg, bytes);

	switch(msg.type){
			// Extra input data from the ARM7
		case PA_MSG_INPUT:
			// Copy the data to the transfer region
			PA_Transfer->tdiode1 = msg.InputMsg.tdiode1;
			PA_Transfer->tdiode2 = msg.InputMsg.tdiode2;
			PA_Transfer->temperature = msg.InputMsg.temperature;
			PA_Transfer->battery = msg.InputMsg.battery;
			PA_Transfer->micvol = msg.InputMsg.micvol;
			PA_Transfer->extra = msg.InputMsg.extra;
			break;
	}
}
