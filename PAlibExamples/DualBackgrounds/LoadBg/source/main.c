// Load a simple background, very easy and simple...

// Includes
#include <PA9.h>       // Include for PA_Lib



// Graphics Includes
// *** Backgrounds ***
// gfx2gba -fsrc -m -pbg.pal -t8 *.bmp

#include "gfx/BG3.map.c" //background files
#include "gfx/BG3.raw.c"
#include "gfx/bg.pal.c" // palette file for the background




// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	// Load Palettes
	PA_LoadPal(PAL_BG0, bg_Palette); // Backgrounds
	PA_LoadPal(PAL_BG1, bg_Palette); // Backgrounds
	
	// Load Backgrounds
	PA_DualLoadSimpleBg(3, // background number (0-3)
						BG3_Tiles, // name of the background tiles given by gfx2gba
						BG3_Map, // same thing for the map
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
