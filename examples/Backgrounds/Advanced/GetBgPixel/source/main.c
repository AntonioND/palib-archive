// Includes
#include <PA9.h>

// Converted using PAGfx
#include "all_gfx.h"

int main(){
	PA_Init();

	PA_LoadDefaultText(1, 0);	

	// Load Backgrounds with their palettes !
	PA_LoadBackground(0, // screen
	                  3, // background number (0-3)
	                  &bg0); // Background name, used by PAGfx...

	PA_OutputSimpleText(1, 2, 10, "Touch the screen to display   the color on top screen");	

	// Infinite loop to keep the program running
	while(true){
		PA_OutputText(1, 2, 5, "Palette Color : %d   ", PA_EasyBgGetPixel(0, 3, Stylus.X, Stylus.Y)); // Screen, Bg_number, X/Y position
		// Returns the palette number (0-255)
		
		PA_SetBgColor(1, PA_EasyBgGetPixelCol(0, 3, Stylus.X, Stylus.Y));  // Same thing, but returns Color value...
		PA_WaitForVBL();
	}
}