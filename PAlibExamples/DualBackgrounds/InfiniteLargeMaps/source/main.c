// Welcome to the magic of large and very large maps ! But here, they wrap around...
// however, these functions are much slower than the normal Large Map functions

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
	
	//PA_LoadSplash();  // PA_Lib splash screen
	
	// Load Palettes
	PA_LoadPal(PAL_BG0, bg0_Pal); // Backgrounds
	PA_LoadPal(PAL_BG1, bg0_Pal); // Backgrounds
	
	// We'll load this huge (1024x1024) background with a special function 
	PA_DualLoadLargeBg(3, // background number
					   Large_Tiles, // Tiles...
					   Large_Map, // Map...
					   1, // 256 color mode
					   128, // width in tiles, 1024/8 => 128
					   128); // Same thing for height...
	
	// Next we'll scroll, here are the variables...
	s32 scrollx = 0; 
	s32 scrolly = 0;
	
	// Infinite loop to keep the program running
while (1)
{

	// We need to change the scroll according to the held keys...
	scrollx += (Pad.Held.Right - Pad.Held.Left)*4; // scroll 4 pixels at a time
	scrolly += (Pad.Held.Down - Pad.Held.Up)*4; // Same thing
	
	PA_DualInfLargeScrollXY(3, // background number
							scrollx, // X scroll
							scrolly); // and Y scroll
	PA_WaitForVBL();
}
	
	return 0;
} // End of main()
