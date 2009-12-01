///////////////////////////////////
// PAlib ARM7_MP3 core           //
//  by fincs                     //
///////////////////////////////////

#include <PA7.h>

void PA_VCount(){
	PA_InputGetAndSend();
}

void PA_VBL(){
	// Legacy IPC
	PA_LegacyIPCManage();

	// ASlib
	AS_SoundVBL();
}

int main(){
	// Initialize PAlib and initialize the sound hardware (for ASlib)
	PA_Init(true);

	// Initialize ASlib
	AS_Init();

	irqSet(IRQ_VBLANK, PA_VBL);
	irqSet(IRQ_VCOUNT, PA_VCount);

	irqEnable(IRQ_VBLANK | IRQ_VCOUNT);

	for(;;){ // Keep the ARM7 mostly idle...
		swiWaitForVBlank();
		AS_MP3Engine(); // ...except for this ;_;
	}
}
