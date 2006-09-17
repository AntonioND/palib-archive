#include <PA9.h>

// PAGfxConverter Include
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"

int main(void){

	PA_Init(); //PAlib inits
	PA_InitVBL();
	
	PA_LoadSpritePal(0, // Screen
					0, // Palette number
					(void*)sprite0_Pal);	// Palette name
					
	// We'll create 2 sprites
	PA_CreateSprite(0, 0, (void*)mollusk_Sprite, OBJ_SIZE_32X32, 1, 0, 0, 50); 
	PA_CreateSprite(0, 1, (void*)mollusk_Sprite, OBJ_SIZE_32X32, 1, 0, 64, 50); 

	PA_SetSpriteMode(0, // Screen
					0, // Sprite
					1); // Alphablending
	

	s16 alpha = 7; // Transparency level

	// Enable the alpha-blending
	PA_EnableSpecialFx(0, // Screen
						SFX_ALPHA, // Alpha blending mode
						0, // Nothing
						SFX_BG0 | SFX_BG1 | SFX_BG2 | SFX_BG3 | SFX_BD); // Everything normal


	while(1) // Infinite loops
	{
		alpha += Pad.Newpress.Up - Pad.Newpress.Down;
		PA_SetSFXAlpha(0, // Screen
						alpha, // Alpha level, 0-15
						15); // Leave this to 15
		
		PA_WaitForVBL();
	}
	
return 0;
}
