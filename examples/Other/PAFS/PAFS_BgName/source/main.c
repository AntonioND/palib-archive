/*  Add backgrounds using PAFS !
*/

// Includes
#include <PA9.h>       // Include for PAlib


// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	PA_InitText(1, 0);  // Initialise the text system on the top screen
	
	PA_OutputText(1, 0, 0, "Loading PAFS...");
	

	

	u32 FileNumber = PA_FSInit(); // Inits PA File System, and returns the number of files

	PA_OutputText(1, 0, 1, "  Files   : %d    ", FileNumber);

	PA_FSBgNameLoad(0, 		// Screen
					0, 		// Bg Number
					"Town");  		// Background name
					
	PA_FSBgNameLoad(1, 1, "Large");	// On the top screen, second background in PAFS (each background has 4 files, so every 4 files...)
	
	
	
	
	s32 scrollx = 0; 
	s32 scrolly = 0;
	
	// Infinite loop to keep the program running
	while (1)
	{ 
			
		scrollx += (Pad.Held.Right - Pad.Held.Left)*4; // scroll 4 pixels at a time
		scrolly += (Pad.Held.Down - Pad.Held.Up)*4; // Same thing
		PA_EasyBgScrollXY(1, 1, scrollx, scrolly);	 // Scroll top background
		PA_EasyBgScrollXY(0, 0, scrollx, scrolly);	 // Scroll bottom background		
		
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()









