// Load an 8 bit bitmap on the DS screen...
// Converted using PAlib Gfx Converter


// Includes
#include <PA9.h>       // Include for PA_Lib

#include "bitmap/all_gfx.c" // Include the picture...
#include "bitmap/all_gfx.h" // Include the picture...

u16 pal[256];

// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	

	// This will initialise an  bit background on each screen. This must be loaded before any other background.
	// If you need to load this after a backgrounds, you'll have to use PA_ResetBgSys, PA_Init8bit, then reload
	// your backgrounds...
	PA_Init8bitBg(0, 3);
	PA_Init8bitBg(1, 3);
	
	PA_LoadNormalBgPal(0, (void*)bitmap_Pal);
	PA_LoadNormalBgPal(1, (void*)bitmap_Pal);
	
	PA_Load8bitBitmap(0, // screen
					Photo8bit_Bitmap); // image
	PA_Load8bitBitmap(1, Photo8bit_Bitmap)
	
	s8 level = 0; // level 0 for normal, 31 would be all white, and -31 all black
	
	PA_InitText(1, 0);
	
	PA_OutputSimpleText(1, 2, 10, "Press Up/Down to change luminosity on the bottom screen");
	
	while(1)
	{	
	   level += Pad.Newpress.Up - Pad.Newpress.Down;   // Adjust level to the pad presses
	   PA_CreatePalBright((u16*)bitmap_Pal, pal, level);   // Create the new palette to load with the custom brightness
	   PA_LoadNormalBgPal(0, (void*)pal);	// Load the bg palette 
	
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
