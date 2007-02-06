// Welcome to the magic of large and very large maps ! You can have as many tiles as you want now !

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

	// Load a large scrolling background converted with PAGfx... InfiniteMap
	PA_EasyBgLoad(0, //screen
						3, //background number (0-3)
						Town); // Name
	
	// Next we'll scroll, here are the variables...
	s32 scrollx = 0; 
	s32 scrolly = 0;
	

// Infinite loop to keep the program running
while (1)
{

	// We need to change the scroll according to the held keys...
	scrollx += (Pad.Held.Right - Pad.Held.Left)*4; // scroll 4 pixels at a time
	scrolly += (Pad.Held.Down - Pad.Held.Up)*4; // Same thing
	PA_EasyBgScrollXY(0, 3, scrollx, scrolly);	
	
	PA_OutputText(1, 2, 5, "Palette Color : %d   ", PA_EasyBgGetPixel(0, 3, Stylus.X, Stylus.Y)); // Screen, Bg_number, X/Y position
	// Returns the palette number (0-255)
		
	PA_SetBgColor(1, PA_EasyBgGetPixelCol(0, 3, Stylus.X, Stylus.Y));  // Same thing, but returns Color value...

	PA_WaitForVBL();
}
	
	return 0;
} // End of main()
