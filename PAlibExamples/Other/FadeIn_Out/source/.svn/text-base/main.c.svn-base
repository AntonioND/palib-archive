// This is basicly an example using the window functions to fade in/out of a screen...
// When fading out with windows, the color shown is the background color...
// This only works on DS, not on emulator !



// Includes
#include <PA9.h>       // Include for PA_Lib



// Graphics Includes
// *** Backgrounds ***
// gfx2gba -fsrc -m -pBackgrounds.pal -t8 BG*.bmp
#include "gfx/BG1.map.c"
#include "gfx/BG1.raw.c"
#include "gfx/BG2.map.c"
#include "gfx/BG2.raw.c"
#include "gfx/BG3.map.c"
#include "gfx/BG3.raw.c"
#include "gfx/Backgrounds.pal.c"




// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	
	// Text...
	PA_InitText(0, 0);
	PA_InitText(1, 0);
	
	// Load Palettes
	PA_LoadPal(PAL_BG0, Backgrounds_Palette); // Backgrounds
	PA_LoadPal(PAL_BG1, Backgrounds_Palette); // Backgrounds
	
	// Load Backgrounds
	PA_LoadSimpleBg(0, 3, BG3_Tiles, BG3_Map, BG_256X256, 0, 1);
	PA_LoadSimpleBg(0, 2, BG2_Tiles, BG2_Map, BG_256X256, 0, 1);
	PA_LoadSimpleBg(0, 1, BG1_Tiles, BG1_Map, BG_256X256, 0, 1);
	PA_LoadSimpleBg(1, 3, BG3_Tiles, BG3_Map, BG_256X256, 0, 1);
	PA_LoadSimpleBg(1, 2, BG2_Tiles, BG2_Map, BG_256X256, 0, 1);
	PA_LoadSimpleBg(1, 1, BG1_Tiles, BG1_Map, BG_256X256, 0, 1);
	
	// We'll change the back color to black...
	PA_SetBgPalCol(0, 0, PA_RGB(0, 0, 0));
	PA_SetBgPalCol(1, 0, PA_RGB(0, 0, 0));
	
	u8 fadetype0 = 0;
	u8 fadetype1 = 0;
	PA_OutputText(0, 10, 10, "Fade : %d  ", fadetype0);
	PA_OutputText(1, 10, 10, "Fade : %d  ", fadetype1);
	s8 i;
	
	// Infinite loop to keep the program running
	while (1)
	{
	
	
		// First we fade out... to fade out, move the time from 0 to 32, 32 included !
		for (i = 0; i <= 32; i++) {
			PA_WindowFade(0, // screen
						  fadetype0, // fade type, from 0 to 7, test them out !
						  i); // Time, 0 being the screen completely visible, 32 completely out
			PA_WindowFade(1, fadetype1, i);
			PA_WaitForVBL(); // To slow down the fades, we wait a frame...
		}


		fadetype0 = PA_Rand()&7; // Random fade type for screen 0... from 0 to 7
		fadetype1 = PA_Rand()&7; // Random fade type for screen 0...
		PA_OutputText(0, 10, 10, "Fade : %d  ", fadetype0);
		PA_OutputText(1, 10, 10, "Fade : %d  ", fadetype1);
		

		// To fade in, same thing, but from 32 to 0 included...
		for (i = 32; i >= 0; i--) {
			PA_WindowFade(0, fadetype0, i);
			PA_WindowFade(1, fadetype1, i);
			PA_WaitForVBL(); // To slow down the fades, we wait a frame...
		}		
						
		fadetype0 = PA_Rand()&7; // Random fade type for screen 0...
		fadetype1 = PA_Rand()&7; // Random fade type for screen 0...
		PA_OutputText(0, 10, 10, "Fade : %d  ", fadetype0);
		PA_OutputText(1, 10, 10, "Fade : %d  ", fadetype1);


		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
