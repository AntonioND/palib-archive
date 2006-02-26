// Load a simple background, very easy and simple...

// Includes
#include <PA9.h>       // Include for PA_Lib



// Converted using PAGfxConverter
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"

u16 pal[256]; // temporary palette

// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	// Load Backgrounds with their palettes !
	PA_LoadTiledBg(0, // screen
					3, // background number (0-3)
					bg0); // Background name, used by PAGfx...
	PA_LoadTiledBg(1, 0, bg0);	

	s8 level = 0; // level 0 for normal, 31 would be all white, and -31 all black
	
	PA_InitText(1, 0);
	
	PA_OutputSimpleText(1, 2, 10, "Press Up/Down to change luminosity on the bottom screen");
	
	while(1)
	{	
	   level += Pad.Newpress.Up - Pad.Newpress.Down;   // Adjust level to the pad presses
	   PA_CreatePalBright((u16*)bg0_Pal, pal, level);   // Create the new palette to load with the custom brightness
	   PA_LoadBgPal(0, 3, (void*)pal);	// Load the bg palette 
	
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
