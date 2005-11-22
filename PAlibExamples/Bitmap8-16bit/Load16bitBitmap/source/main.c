// Load an 16 bit bitmap on the DS screen...
// Gfx2gba command line : gfx2gba -D -fsrc -c32k *.BMP


// Includes
#include <PA9.h>       // Include for PA_Lib

#include "bitmap/all_gfx.c" // Include the picture...
#include "bitmap/all_gfx.h" // Include the picture...

// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	

	// This will initialise a 16 bit background on each screen. This must be loaded before any other background.
	// If you need to load this after a backgrounds, you'll have to use PA_ResetBgSys, PA_Init8bit, then reload
	// your backgrounds...
	PA_Init16bitBg(0, 3);
	PA_Init16bitBg(1, 3);
	
		// No need to load a palette !
		
	PA_Load16bitBitmap(0, // screen
					Photo16bit_Bitmap); // image
	PA_Load16bitBitmap(1, Photo16bit_Bitmap)
	
	// Infinite loop to keep the program running
	while (1)
	{
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
