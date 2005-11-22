// Load a simple background, very easy and simple...

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
	
	// Load Palettes
	PA_LoadPal(PAL_BG0, bg0_Pal); // Backgrounds
	PA_LoadPal(PAL_BG1, bg0_Pal); // Backgrounds
	
	// Load Backgrounds
	PA_DualLoadSimpleBg(3, // background number (0-3)
						bg0_Tiles, // name of the background tiles given by gfx2gba
						bg0_Map, // same thing for the map
						BG_256X256, // background size (can also be 512X256, 256X512, or 512X512
						0, // Leave this to 0 :)  only used for rotating backgrounds
						1); // 256 color mode... set to 0 for 16 color mode

	

	// Infinite loop to keep the program running
	while (1)
	{
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
