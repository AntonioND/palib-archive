// Load a simple background, very easy and simple...

// Includes
#include <PA9.h>       // Include for PA_Lib


// Converted using PAGfx
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"



// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL


	PA_InitText(1, 0);	
	
	// Load Backgrounds with their palettes !
	PA_EasyBgLoad(0, // screen
					3, // background number (0-3)
					bg0); // Background name, used by PAGfx...
	
	PA_OutputSimpleText(1, 2, 10, "Touch the screen to display   the color on top screen");	

	// Infinite loop to keep the program running
	while (1)
	{
		PA_OutputText(1, 2, 5, "Palette Color : %d   ", PA_EasyBgGetPixel(0, 3, Stylus.X, Stylus.Y)); // Screen, Bg_number, X/Y position
		// Returns the palette number (0-255)
		
		PA_SetBgColor(1, PA_EasyBgGetPixelCol(0, 3, Stylus.X, Stylus.Y));  // Same thing, but returns Color value...
		PA_WaitForVBL();

	}
	
	return 0;
} // End of main()
