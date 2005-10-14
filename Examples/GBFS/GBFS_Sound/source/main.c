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

	u8 nfiles = PA_InitGBFS(); // Init GBFS and get the number of files...
	PA_OutputText(1, 0, 0, "Files : %d", nfiles); // Show the number of files we added

	while(1){	// Infinite loop
	// There are 3 ways of loading sounds from GBFS (the same as for normal sounds) : 
	// PA_PlayGBFSSimpleSound, PA_PlayGBFSSound, and PA_PlayGBFSSoundEx
		if (Pad.Newpress.A) PA_PlayGBFSSimpleSound(0, 0); // Sound channel and number in GBFS
		if (Pad.Newpress.B) PA_PlayGBFSSound(0, 0, 127, 11025); // Sound channel and number in GBFS, sound volume and rate...

		PA_WaitForVBL();
	}
		
	return 0;
} // End of main()


