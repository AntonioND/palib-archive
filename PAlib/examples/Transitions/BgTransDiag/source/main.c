// This is basicly an example using the window functions to fade in/out of a screen...
// When fading out with windows, the color shown is the background color...
// This only works on DS, not on emulator !



// Includes
#include <PA9.h>       // Include for PA_Lib



#include "gfx/all_gfx.h"
#include "gfx/all_gfx.c"



// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	PA_InitText(1, 0);
	
	PA_EasyBgLoad(0, 1, pasplash); // Load your backgrounds...
	
	PA_InitBgTrans(0); // Init BgTransition system, uses background 0 but little memory...
	// If you want it to hide your sprites, set your sprites' priorities to 1 or more...
	
	u8 transtype = 0;

	s8 i;
	u8 hflip, vflip;
	
	// Infinite loop to keep the program running
	while (1)
	{
		hflip = PA_Rand()&1; // random
		vflip = PA_Rand()&1; // random		
		transtype = PA_Rand()%5; // random
		
		PA_OutputText(1, 8, 8, "Transition : %d ", transtype);
		PA_OutputText(1, 6, 9, "Hflip : %d   Vflip : %d  ", hflip, vflip);				
	
		// Transition out...
		for (i = 0; i <= TRANS_LENGTH; i++) { // Fade length...
			PA_BgTransDiag(0, // screen
						  transtype, // fade type, from 0 to 4, test them out !
						  hflip, // horizontal flip
						  vflip, // vertical flip						  
						  i); // Time, 0 being the screen completely visible, 32 completely out
			PA_WaitForVBL(); // To slow down the fades, we wait a frame...
		}
		
		hflip = PA_Rand()&1; // random
		vflip = PA_Rand()&1; // random	
		transtype = PA_Rand()%5; // random
		
		PA_OutputText(1, 8, 8, "Transition : %d ", transtype);
		PA_OutputText(1, 6, 9, "Hflip : %d   Vflip : %d  ", hflip, vflip);			
			
		// Transition back in...
		for (i = TRANS_LENGTH; i >= 0; i--) { // Fade length...
			PA_BgTransDiag(0, transtype, hflip, vflip, i);// same thing...
			PA_WaitForVBL(); // To slow down the fades, we wait a frame...
		}		


		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
