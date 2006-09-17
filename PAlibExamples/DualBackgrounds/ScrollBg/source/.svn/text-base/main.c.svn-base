// This time around, we'll scroll a background...

// Includes
#include <PA9.h>       // Include for PA_Lib



// Converted using PAGfxConverter
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"



// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	// Load the dual background...
	PA_DualLoadTiledBg(3, //bg number
						BG3);	//background name

	s32 scrollx = 0; // No X scroll by default...
	s32 scrolly = 0; // No Y scroll by default...

	// Infinite loop to keep the program running
	while (1)
	{
		// We'll modify scrollx and scrolly according to the keys pressed
		scrollx += (Pad.Held.Left - Pad.Held.Right) * 4; // Move 4 pixels per press
		scrolly += (Pad.Held.Up - Pad.Held.Down) * 4; // Move 4 pixels per press

		// Scroll the background to scrollx, scrolly...
		PA_DualBGScrollXY(3, // Background number
						scrollx, // X scroll
						scrolly); // Y scroll
		

		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
