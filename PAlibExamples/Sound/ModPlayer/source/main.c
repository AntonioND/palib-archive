// Play a simple mod !

// Includes
#include <PA9.h>       // Include for PA_Lib
#include "modfile.h"  // Include the mod file (the .mod file is in the data directory)


// Function: main()
int main(int argc, char ** argv){

	PA_Init();    // PA Init...
	PA_InitVBL();	// VBL Init...
	
	PA_InitSound(); 	// Sound Init, for the mod player...
	PA_InitText(1, 0);

	PA_PlayMod(modfile);  		// Play a given mod

	while(1){	// Infinite loop
		PA_WaitForVBL();
	}
		
	return 0;
} // End of main()


