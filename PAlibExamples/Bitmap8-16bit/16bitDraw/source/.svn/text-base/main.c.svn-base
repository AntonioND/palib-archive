// Drawing on DS is easy... This'll show you to what point !

// Includes
#include <PA9.h>       // Include for PA_Lib



// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	

	
	// Let's not load any backgrounds :p
	PA_SetBgPalCol(0, 0, PA_RGB(31, 31, 31)); // Set the bottom screen color to white
	PA_SetBgPalCol(1, 0, PA_RGB(0, 0, 0));	// set the top screen color to black
	
	// This will initialise a 16 bit background on each screen. This must be loaded before any other background.
	// If you need to load this after a backgrounds, you'll have to use PA_ResetBgSys, PA_Init8bit, then reload
	// your backgrounds...
	PA_Init16bitBg(0, 3);
	PA_Init16bitBg(1, 3);
	
	
	
	// Infinite loop to keep the program running
	while (1)
	{
		// Simple draw function, draws on the screen...
		PA_16bitDraw(0, // Screen
					PA_RGB(31, 0, 0));  // Color : full red...
		// (The first shooting line on emu is a bug from the emu, I don't have it on DS)	
					
		PA_Put16bitPixel(1, Stylus.X, Stylus.Y, PA_RGB(0, 31, 0)); // Draws a pixel on the top screen, corresponding to the stylus's position
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
