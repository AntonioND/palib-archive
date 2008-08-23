// This example shows how to use 8 bit backgrounds, 
// on which you can draw with the stylus or show text in different sizes, colors, or rotations

// Includes
#include <PA9.h>       // Include for PA_Lib



// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	//PA_LoadSplash();  // PA_Lib splash screen
	
	// Let's put some colors... in the background palettes
	PA_SetBgPalCol(0, 1, PA_RGB(31, 31, 31));
	PA_SetBgPalCol(0, 2, PA_RGB(31, 0, 0));	
	PA_SetBgPalCol(0, 3, PA_RGB(0, 0, 31));
	PA_SetBgPalCol(0, 4, PA_RGB(0, 31, 0));
	PA_SetBgPalCol(1, 1, PA_RGB(31, 31, 31));
	PA_SetBgPalCol(1, 2, PA_RGB(31, 0, 0));	
	PA_SetBgPalCol(1, 3, PA_RGB(0, 0, 31));
	PA_SetBgPalCol(1, 4, PA_RGB(0, 31, 0));
	
	// Let's not load any backgrounds :p
	
	// This will initialise an  bit background on each screen. This must be loaded before any other background.
	// If you need to load this after a backgrounds, you'll have to use PA_ResetBgSys, PA_Init8bit, then reload
	// your backgrounds...
	PA_Init8bitBg(0, 3);
	PA_Init8bitBg(1, 3);
	
	// On these backgrounds, you can plot pixels, draw lines... and draw some custom text !
	PA_SmartText(1, // SCreen
				0, // Top left X
				1, // Top left Y
				255, // Botton right X
				20, // Bottom right Y
				"Hello World", // Text
				1, // Text palette color (0-255)
				0, // Text size (0-4)
				1, // Text mode (0 - normal, 1 - transparent, 2 - no text, 3 - rotated 90°, 4 - 90° other way)
				100); // Maximum number of characters, if you want to right letter by letter...
	PA_SmartText(1, 0, 20, 255, 40, "Hello World", 2, 1, 1, 100);
	PA_SmartText(1, 0, 40, 255, 60, "Hello World", 3, 2, 1, 100);
	PA_SmartText(1, 0, 60, 255, 80, "Hello World", 4, 3, 1, 100);
	PA_SmartText(1, 0, 80, 255, 100, "Hello World", 1, 4, 1, 100);


	// Let's have some rotated text :p
	// X and Y positions are taken as if the DS was turned
	PA_SmartText(1, 1, 1, 190, 20, "Hello World", 3, 3, 3, 100);

	// And a centered text, to check if it works
	PA_CenterSmartText(1, 0, 100, 255, 120, "Hello World", 2, 4, 1); // Same as Smart text, but no letter limit
		
	
	
	
	// Infinite loop to keep the program running
	while (1)
	{
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
