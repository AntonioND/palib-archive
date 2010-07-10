///////////////////////////////////
// PAlib ARM7_MAXMOD_DSWIFI core //
//  by fincs                     //
///////////////////////////////////

#include <PA7.h>

void PA_VCount(){
	PA_InputGetAndSend();
}

void PA_VBL(){
	// Legacy IPC
	PA_LegacyIPCManage();

	// Wifi
	Wifi_Update();
}

int main(){
	// Initialize PAlib but don't initialize the sound hardware
	PA_Init(false);

	// Initialize Wifi
	installWifiFIFO();

	// Initialize Maxmod
	mmInstall(FIFO_MAXMOD);

	// Basic services...
	irqSet(IRQ_VBLANK, PA_VBL);
	irqSet(IRQ_VCOUNT, PA_VCount);

	irqEnable(IRQ_VBLANK | IRQ_VCOUNT);

	for(;;) // Keep the ARM7 mostly idle...
		swiWaitForVBlank();
}
