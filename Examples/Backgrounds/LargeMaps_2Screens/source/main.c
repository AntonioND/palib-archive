// Welcome to the magic of large and very large maps !
// This time we'll load on both screens, and have them correspond perfectly

// Includes
#include <PA9.h>       // Include for PA_Lib



// Graphics Includes
// *** Backgrounds ***
// Be carefull, it's not exactly the same gfx2gba command... 
// gfx2gba -fsrc -m -pbg.pal -t8 *.bmp 
#include "gfx/Large.map.c"
#include "gfx/Large.raw.c"
#include "gfx/bg.pal.c"




// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	//PA_LoadSplash();  // PA_Lib splash screen
	
	// Load Palettes
	PA_LoadPal(PAL_BG0, bg_Palette); // Backgrounds
	PA_LoadPal(PAL_BG1, bg_Palette); // Backgrounds
	
	// We'll load this huge (1024x1024) background with a special function 
	PA_LoadLargeBg(1, // screen
				   3, // background number
				   Large_Tiles, // Tiles...
				   Large_Map, // Map...
				   1, // 256 color mode
				   128, // width in tiles, 1024/8 => 128
				   128); // Same thing for height...
				   
	PA_WaitForVBL(); // It takes so long to load a bg, that apparently it hangs if you don't wait a little in between !
	
	PA_LoadLargeBg(0, 3, Large_Tiles, Large_Map, 1, 128, 128); // Same, for screen 1...	
	
	// Next we'll scroll, here are the variables...
	s32 scrollx = 0; 
	s32 scrolly = 0;
	
	// Infinite loop to keep the program running
while (1)
{

	// We need to change the scroll according to the held keys...
	scrollx += (Pad.Held.Right - Pad.Held.Left)*4; // scroll 4 pixels at a time
	scrolly += (Pad.Held.Down - Pad.Held.Up)*4; // Same thing
	
	//We'll limit the range of the scroll...
	if (scrollx < 0) scrollx = 0;		if (scrollx >= 1024-256) scrollx = 1024-256;
	if (scrolly < 0) scrolly = 0;		if (scrolly >= 1024-192-192) scrolly = 1024-192-192;	
	
	
	PA_LargeScrollXY(1, // screen
					3, // background number
					scrollx, // X scroll
					scrolly); // and Y scroll
	
	PA_LargeScrollXY(0, 3, scrollx, scrolly+192); // +192, because that's the height of the screen...				
	PA_WaitForVBL();
}
	
	return 0;
} // End of main()
