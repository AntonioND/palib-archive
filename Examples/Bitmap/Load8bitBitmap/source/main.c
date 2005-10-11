// Load an 8 bit bitmap on the DS screen...
// Gfx2gba command line : gfx2gba -D -fsrc -pbitmap.pal *.bmp


// Includes
#include <PA9.h>       // Include for PA_Lib

#include "bitmap/Photo8bit.raw.c" // Include the picture...
#include "bitmap/bitmap.pal.c"  // Include the picture's palette...

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
	
	PA_LoadPal(PAL_BG0, bitmap_Palette);
	PA_LoadPal(PAL_BG1, bitmap_Palette);
	
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
