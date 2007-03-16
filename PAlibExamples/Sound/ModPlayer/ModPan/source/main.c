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
	PA_OutputText(0, 5, 11, "Move Stylus to change pan");

	u8 i;
	
	while(1){	// Infinite loop
		
		for(i = 0; i < 3; i++){
			PA_OutputText(0, 6, 6+i*6, "Mod Channel %d Pan : %d  ", i, PA_IPC.Mod.Pan[i]); 
			if(Stylus.Held && PA_StylusInZone(0, (6+i*6)*8-24, 255, 	(6+i*6)*8+24)) PA_SetModChanPan(i, Stylus.X>>1); 
		}			
 
		
		PA_WaitForVBL();

	}
		
	return 0;
} // End of main()


