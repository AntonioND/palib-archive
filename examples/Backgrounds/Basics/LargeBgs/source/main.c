// Includes
#include <PA9.h>

// Converted using PAGfxConverter
#include "all_gfx.h"

int main(){
	PA_Init();

	// Load a large scrolling background converted with PAGfx... 
	PA_LoadBackground(0,       // Screen
	                  3,       // Background number (0-3)
	                  &zelda); // Name

	// Next we'll scroll, here are the variables...
	s32 scrollx = 0; 
	s32 scrolly = 0;
	
	// Infinite loop to keep the program running
	while(true){
		PA_WaitForVBL();

		// We need to change the scroll according to the held keys...
		scrollx += (Pad.Held.Right - Pad.Held.Left)*4; // scroll 4 pixels at a time
		scrolly += (Pad.Held.Down - Pad.Held.Up)*4; // Same thing

		if(scrollx < 0) scrollx = 0;
		if(scrollx > (2048-256)) scrollx = (2048-256);

		if(scrolly < 0) scrolly = 0;
		if(scrolly > (2048-192)) scrolly = (2048-192);
		
		PA_EasyBgScrollXY(0, // screen
		                  3, // background number
		                  scrollx, // X scroll
		                  scrolly); // and Y scroll

	}
}