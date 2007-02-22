// Welcome to the magic of large and very large maps ! (2048x2048 in this example)

// Includes
#include <PA9.h>       // Include for PA_Lib

// Converted using PAGfxConverter
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"


// Settings to change (ingame) in order to adjust width/height of the sine wave
s8 width = 8;
s8 height = 8;
u16 add = 0;
s32 scrollx = 0;  // Scroll positions...
s32 scrolly = 0;


void HBL_function(void){
   s16 vcount = PA_GetVcount();
   vcount++;
   if(vcount > 192) vcount = 0; // Get correct vcount
   
   PA_BGScrollX(0, 3, (scrollx&511) + ((PA_Sin((vcount+scrolly)*height+add)*width)>>8)); // Sin wav, variable width, height, and add to change start point ;)
}   

// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	PA_InitText(1, 0);
	
	PA_OutputSimpleText(1, 9, 10, "Sine Bg example");
	
	// Load a large scrolling background converted with PAGfx... 
	PA_EasyBgLoad(0, //screen
						3, //background number (0-3)
						zelda); // Name
	
	irqSet(IRQ_HBLANK, HBL_function);
	irqEnable(IRQ_HBLANK);
		
	// Next we'll scroll, here are the variables...
	scrollx = 0; 
	scrolly = 0;
	
	// Infinite loop to keep the program running
	while (1)
	{
	
		add+=4; add&= 511;	// Move wave...
	
		// We need to change the scroll according to the held keys...
		scrollx += (Pad.Held.Right - Pad.Held.Left)*4; // scroll 4 pixels at a time
		if(scrollx < 0) scrollx = 0; if(scrollx > 2047-256) scrollx = 2047-256;
		scrolly += (Pad.Held.Down - Pad.Held.Up)*4; // Same thing
		if(scrolly < 0) scrolly = 0; if(scrolly > 2047-192) scrolly = 2047-192;
		
		PA_LargeScrollXY(0, // screen
						3, // background number
						scrollx, // X scroll
						scrolly); // and Y scroll
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
