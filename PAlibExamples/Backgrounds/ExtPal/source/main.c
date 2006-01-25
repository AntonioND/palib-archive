// This example shows you how to use extended palettes for backgrounds, which gives you 1 palette per background... way better :p Notice that here, each background has his palette...

// Includes
#include <PA9.h>       // Include for PA_Lib



// Graphics converted using PAGfxConverter
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"

// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL

	
	PA_InitBgExtPal(); // Extended palettes init...
	
	// We'll now load the 4 palettes, one for each background
	PA_LoadBgExtPal(0, // Screen
					0, // Background number
					(void*)bg0_Pal); // Palette to load
	PA_LoadBgExtPal(0, 1, (void*)bg1_Pal); // Palette to load
	PA_LoadBgExtPal(0, 2, (void*)bg2_Pal); // Palette to load
	PA_LoadBgExtPal(0, 3, (void*)bg3_Pal); // Palette to load	

	// Load the 4 Backgrounds
	PA_LoadSimpleBg(0, 0, bg0_Tiles, bg0_Map, BG_256X256, 0, 1); 
	PA_LoadSimpleBg(0, 1, bg1_Tiles, bg1_Map, BG_256X256, 0, 1); 
	PA_LoadSimpleBg(0, 2, bg2_Tiles, bg2_Map, BG_256X256, 0, 1); 
	PA_LoadSimpleBg(0, 3, bg3_Tiles, bg3_Map, BG_256X256, 0, 1); 	
	
	// Infinite loop to keep the program running
	while (1)
	{
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
