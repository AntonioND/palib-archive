// Welcome to the magic of large and very large maps! You can have as many tiles as you want now!

// Includes
#include <PA9.h>

// Converted using PAGfx
#include "all_gfx.h"

int main(){
	PA_Init();

	PA_LoadDefaultText(1, 0);

	// Load a UnlimitedBg converted with PAGfx...
	PA_LoadBackground(0,      // Screen
	                  3,      // Background number (0-3)
	                  &Town); // "Name" (pointer to background struct)

	// Next we'll scroll, here are the variables...
	int scrollx = 0; 
	int scrolly = 0;

	// Infinite loop to keep the program running
	while(true){
		PA_WaitForVBL();

		// We need to change the scroll according to the held keys...
		scrollx += (Pad.Held.Right - Pad.Held.Left)*4; // scroll 4 pixels at a time
		scrolly += (Pad.Held.Down - Pad.Held.Up)*4; // Same thing
		PA_EasyBgScrollXY(0, 3, scrollx, scrolly);	
	}
}