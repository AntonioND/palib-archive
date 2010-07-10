#include <PA9.h>

// PAGfxConverter Include
#include "all_gfx.h"



int main(void){

	PA_Init(); //PAlib inits
	
	PA_LoadSpritePal(0, // Screen
					0, // Palette number
					(void*)sprite0_Pal);	// Palette name

	// Create a blank buffer. It's the same as saying u32 blankdata[1024], but you can free the memory later.
	void* blankdata = calloc(1024, sizeof(u32)); // get the hint? calloc(size of array, sizeof(type));

	PA_CreateSprite(0, 0, (void*)blankdata, // Blank sprite !
					OBJ_SIZE_64X64, // Sprite size
					1, 0, // Sprite palette number
					0, 0); // In the upper left corner

	// Free the buffer.
	free(blankdata);

	PA_SetSpriteRotEnable(0, 0, 0); // enable rotation
	PA_SetSpriteDblsize(0, 0, 1); // enable double size
	PA_SetRotset(0, 0, 0, 128, 128); // zoom to fill the double size
		

	PA_InitAllSpriteDraw(); // Init the draw system for all onscreen sprites
	
	
	while(1) // Infinite loops
	{
	   if ((Stylus.X < 128) && (Stylus.Y < 128))	   PA_SetSpritePixel(0, 0, Stylus.X>>1, Stylus.Y>>1, 10); 	 
		// >> 1, meaning divided by 2, because our sprite is double sized using the zoom !
	   PA_WaitForVBL();
	   
	
	}
	
return 0;
}