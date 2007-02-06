// Play a simple mod !

// Includes
#include <PA9.h>       // Include for PA_Lib
#include "modfile.h"  // Include the mod file (the .mod file is in the data directory)


// Function: main()
int main(int argc, char ** argv){

	PA_Init();    // PA Init...
	PA_InitVBL();	// VBL Init...
	
	PA_InitSound(); 	// Sound Init, for the mod player...
	PA_InitText(0, 0);  PA_InitText(1, 0);

	PA_PlayMod(modfile);  		// Play a given mod
	
	PA_OutputText(0, 8, 10, "Playing Mod...");
u8 i;
	while(1){	// Infinite loop
		// See which channels are busy
		for (i = 0; i < 16; i++) PA_OutputText(1, 0, i, "Channel %02d busy : %d  ", i, PA_SoundChannelIsBusy(i));	 // 0 for free, 1 for busy
		
		PA_WaitForVBL();
	}
		
	return 0;
} // End of main()


