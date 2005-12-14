// Includes
#include <PA9.h>       // Include for PA_Lib

// Converted using PAGfxConverter
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"

// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	// Load the 4 Backgrounds on the bottom screen...
	PA_LoadTiledBg(0, 1, BG1);	//screen, background number, background name
	PA_LoadTiledBg(0, 2, BG2);	
	PA_LoadTiledBg(0, 3, BG3);	

	// Initialise parallax vertically (Y axis) for both backgrounds
	// 256 is normal speed, 128 half speed, 512 twice as fast...
	PA_InitParallaxY(0, //screen
					0, //Parallax speed for Background 0. 0 is no parallax (will scroll independently with BGScroll)
					256, // Normal speed for Bg1
					192, //   3/4 speed
					128); // Half speed
	PA_InitParallaxY(1, 0, 256, 192, 128);	// Same thing, but for SCreen 1...
	
	s32 scroll = 0;

	
	// Infinite loop to keep the program running
	while (1)
	{
		scroll += 1; // Scroll by one pixel... 
		// Backgrounds with a parallax speed of 256 will scroll 1 pixel, 192 will scroll 0.75, and 128 0.5
		// We could also have put a negative parallax speed to have some backgrounds scroll in different directions	
		
		PA_ParallaxScrollY(0, -scroll);  // Scroll the screen 0 backgrounds. 

		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
