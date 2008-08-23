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
	
	u8 fadetype0 = 0;

	s8 i;
	
	// Infinite loop to keep the program running
	while (1)
	{
	
	
		// First we fade out... to fade out, move the time from 0 to 32, 32 included !
		for (i = 0; i <= 32; i++) {
			PA_WindowFade(0, // screen
						  fadetype0, // fade type, from 0 to 7, test them out !
						  i); // Time, 0 being the screen completely visible, 32 completely out
			PA_WaitForVBL(); // To slow down the fades, we wait a frame...
		}


		fadetype0 = PA_Rand()&7; // Random fade type for screen 0... from 0 to 7
		PA_OutputText(1, 8, 9, "Window Fade : %d  ", fadetype0);
		

		// To fade in, same thing, but from 32 to 0 included...
		for (i = 32; i >= 0; i--) {
			PA_WindowFade(0, fadetype0, i);
			PA_WaitForVBL(); // To slow down the fades, we wait a frame...
		}		
						
		fadetype0 = PA_Rand()&7; // Random fade type for screen 0...
		PA_OutputText(1, 8, 9, "Window Fade : %d  ", fadetype0);



		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
