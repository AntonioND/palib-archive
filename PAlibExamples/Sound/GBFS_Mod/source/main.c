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

	u8 nfiles = PA_InitGBFS(); // Init GBFS and get the number of files...
	PA_OutputText(1, 0, 0, "Files : %d", nfiles); // Show the number of files we added
		
	PA_PlayGBFSMod(0); // Since only the mod is included, it's in file position 0...
	
	while(1){	// Infinite loop
		PA_WaitForVBL();
	}
		
	return 0;
} // End of main()


