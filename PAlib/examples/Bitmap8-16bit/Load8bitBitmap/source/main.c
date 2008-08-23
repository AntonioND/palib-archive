// Load an 8 bit bitmap on the DS screen...
// Converted using PAlib Gfx Converter


// Includes
#include <PA9.h>       // Include for PA_Lib

#include "bitmap/all_gfx.c" // Include the picture...
#include "bitmap/all_gfx.h" // Include the picture...

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
	
	PA_Load8bitBgPal(0, (u8*)bitmap_Pal);
	PA_Load8bitBgPal(1, (u8*)bitmap_Pal);
	
	PA_Load8bitBitmap(0, // screen
					Photo8bit_Bitmap); // image
	PA_Load8bitBitmap(1, Photo8bit_Bitmap)
	
	// Infinite loop to keep the program running
	while (1)
	{
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
