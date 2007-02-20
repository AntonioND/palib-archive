
// Includes
#include <PA9.h>
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"

// Main function
int main(void)	{

	PA_Init();
	PA_InitVBL();
	
	PA_InitText(1, 0);
	
	PA_LoadSpritePal(0, 0, (void*)sprite0_Pal);	// Load the sprite palette
	
	// Load a sprite...
	PA_CreateSprite(0, 0,(void*)frisbee_Sprite, OBJ_SIZE_32X32,1, 0, 128-16, 96-16);  //Create a sprites
	PA_InitAllSpriteDraw();  // Inits the sprite draw mode, necessary for the pixel-perfect touch detection

	
	while(1)
	{	
		// If we touch the sprite's pixel, returns 1...
		if (PA_SpriteTouchedPix(0)) PA_OutputText(1, 6, 15, "Touching...   ");
		else PA_OutputText(1, 6, 15, "Try again !   ");
				
		PA_WaitForVBL();
	}
	return 0;
}
