// Drawing on DS is easy... This'll show you to what point !

// Includes
#include <PA9.h>       // Include for PA_Lib



// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	// Let's put some colors... in the background palettes
	PA_SetBgPalCol(0, 1, PA_RGB(31, 31, 31));
	PA_SetBgPalCol(0, 2, PA_RGB(31, 0, 0));	
	PA_SetBgPalCol(0, 3, PA_RGB(0, 0, 31));
	PA_SetBgPalCol(0, 4, PA_RGB(0, 31, 0));
	PA_SetBgPalCol(1, 1, PA_RGB(31, 31, 31));
	PA_SetBgPalCol(1, 2, PA_RGB(31, 0, 0));	
	PA_SetBgPalCol(1, 3, PA_RGB(0, 0, 31));
	PA_SetBgPalCol(1, 4, PA_RGB(0, 31, 0));
	

	// This will initialise an  bit background on each screen. This must be loaded before any other background.
	// If you need to load this after a backgrounds, you'll have to use PA_ResetBgSys, PA_Init8bit, then reload
	// your backgrounds...
	PA_Init8bitBg(0, 3);
	PA_Init8bitBg(1, 3);
		

	// Infinite loop to keep the program running
	while (1)
	{
		// Simple draw function, draws on the screen...
		PA_8bitDraw(0, // Screen
					1);  // Palette color
					
		PA_Put8bitPixel(1, Stylus.X, Stylus.Y, 2); // Draws a pixel on the top screen, corresponding to the stylus's position

		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
