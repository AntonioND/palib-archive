// This time around, we'll scroll a background...

// Includes
#include <PA9.h>

// Converted using PAGfx
#include "all_gfx.h"

int main(){
	PA_Init();
	
	// Load the dual background...
	PA_DualLoadBackground(3,     //bg number
	                      &BG3); //background name

	s32 scrollx = 0; // No X scroll by default...
	s32 scrolly = 0; // No Y scroll by default...

	// Infinite loop to keep the program running
	while(true){
		// We'll modify scrollx and scrolly according to the keys pressed
		scrollx += (Pad.Held.Left - Pad.Held.Right) * 4; // Move 4 pixels per press
		scrolly += (Pad.Held.Up - Pad.Held.Down) * 4; // Move 4 pixels per press

		// Scroll the background to scrollx, scrolly...
		PA_DualBGScrollXY(3, // Background number
						scrollx, // X scroll
						scrolly); // Y scroll
		

		PA_WaitForVBL();
	}
}