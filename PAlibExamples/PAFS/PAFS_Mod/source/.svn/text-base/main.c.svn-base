// Play a simple mod, included in GBFS
// Check the GBFS examples for more info

// Includes
#include <PA9.h>       // Include for PA_Lib

// Function: main()
int main(int argc, char ** argv){

	PA_Init();    // PA Init...
	PA_InitVBL();	// VBL Init...
	
	PA_InitSound(); 	// Sound Init, for the mod player...
	
	PA_InitText(1, 0);  // Initialise the text system on the top screen

	u32 nfiles = PA_FSInit(); // Inits PA File System, and returns the number of files
	PA_OutputText(1, 0, 0, "Files : %d", nfiles); // Show the number of files we added

	PA_PlayFSMod(0);	// Play the mod in PAFS, slot 1...
	
	while(1){	// Infinite loop
	
		if (Pad.Newpress.A){
			//file++;
			//for (i = 0; i < length<<2; i++) FS_mod[i] = file[i];
			PA_PlayMod(FS_mod);
		}

		PA_WaitForVBL();
	}
		
	return 0;
} // End of main()


