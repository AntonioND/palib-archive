#include <PA9.h>

// Converted using PAGfx
#include "all_gfx.h"

int main(){
	PA_Init();

	PA_LoadDefaultText(1, 0);

	// Load a background converted with PAGfx... InfiniteMap
	PA_LoadBackground(0, //screen
	                  3, //background number (0-3)
	                  &Town); // Name
	
	// Next we'll scroll, here are the variables...
	s32 scrollx = 0; 
	s32 scrolly = 0;

	// Infinite loop to keep the program running
	while(true){
		// We need to change the scroll according to the held keys...
		scrollx += (Pad.Held.Right - Pad.Held.Left)*4; // scroll 4 pixels at a time
		scrolly += (Pad.Held.Down - Pad.Held.Up)*4; // Same thing
		PA_EasyBgScrollXY(0, 3, scrollx, scrolly);	
		
		PA_OutputText(1, 2, 5, "Palette Color: %d   ", PA_EasyBgGetPixel(0, 3, Stylus.X, Stylus.Y)); // Screen, Bg_number, X/Y position
		// Returns the palette number (0-255)
			
		PA_SetBgColor(1, PA_EasyBgGetPixelCol(0, 3, Stylus.X, Stylus.Y));  // Same thing, but returns Color value...

		PA_WaitForVBL();
	}
}