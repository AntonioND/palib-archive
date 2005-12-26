// Play a simple sound included in GBFS
// Check the GBFS examples for more info

// Includes
#include <PA9.h>       // Include for PA_Lib



// Function: main()
int main(int argc, char ** argv){

	PA_Init();    // PA Init...
	PA_InitVBL();	// VBL Init...
	
	
	PA_InitSound();  // Init the sound system	

	PA_InitText(1, 0);  // Initialise the text system on the top screen

	u8 nfiles = PA_FSInit(); // Inits PA File System, and returns the number of files
	PA_OutputText(1, 0, 0, "Files : %d", nfiles); // Show the number of files we added
	
	
	while(1){	// Infinite loop
	// There are 3 ways of loading sounds from GBFS (the same as for normal sounds) : 
	// PA_PlayFSSimpleSound, PA_PlayFSSound, and PA_PlayFSSoundEx
		
		if(Pad.Newpress.A)	PA_PlayFSSimpleSound(0, 0); // Play a standard sound on channel 0, sound file 0 in PAFS
	
		PA_WaitForVBL();

	}
		
	return 0;
} // End of main()


