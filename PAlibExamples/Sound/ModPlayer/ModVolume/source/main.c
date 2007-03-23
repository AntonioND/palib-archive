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

	
	while(1){	// Infinite loop
		
		PA_OutputText(1, 6, 6, "Mod Volume : %d  ", PA_GetModVolume()); 
		PA_OutputText(1, 2, 7, "Move Stylus to change volume"); 		
		
		if(Stylus.Held) {
		   PA_SetModVolume(Stylus.X >> 1);
		}  
		
		PA_WaitForVBL();

	}
		
	return 0;
} // End of main()


