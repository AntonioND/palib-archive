/* This shows how to change a sprite's frame...
*/


// Includes, only one sprite
#include <PA9.h>
// PAGfxConverter Include
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"


s16 framecount[4]; //Frame count for 4 different sprites...


// Main function
int main(void){

	// PAlib init
	PA_Init();
	PA_InitVBL();
	
	// Load the sprite palette
		PA_LoadSpritePal(0, // Screen
						0, // Palette number
						(void*)sprite0_Pal);	// Palette name
	
	
	// Load a sprites... We'll then change the frame...
	PA_CreateSprite(0, 0,(void*)SpaceShipAnim_Sprite, OBJ_SIZE_64X64,1, 0, 64, 64);
	
	
	while(1)
		{
		
			if (Pad.Newpress.Left) PA_SetSpriteAnim(0, // Screen
												0, // Sprite to animate
												0); // frame 0, to test...
			if (Pad.Newpress.Right) PA_SetSpriteAnim(0, 0, 1); // frame 1, to test...	
			if (Pad.Newpress.Up) PA_SetSpriteAnim(0, 0, 2); // frame 2, to test...	
			if (Pad.Newpress.Down) PA_SetSpriteAnim(0, 0, 3); // frame 3, to test...	

			PA_WaitForVBL();
		}
	return 0;
}
