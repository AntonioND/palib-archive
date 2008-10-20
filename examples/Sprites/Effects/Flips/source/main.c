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
					
	// We'll create 4 identical sprites, and flips them all differently...
	PA_CreateSprite(0, 0, (void*)mollusk_Sprite, OBJ_SIZE_32X32, 1, 0, 0, 50); 
	PA_CreateSprite(0, 1, (void*)mollusk_Sprite, OBJ_SIZE_32X32, 1, 0, 64, 50); 
	PA_CreateSprite(0, 2, (void*)mollusk_Sprite, OBJ_SIZE_32X32, 1, 0, 128, 50); 	
	PA_CreateSprite(0, 3, (void*)mollusk_Sprite, OBJ_SIZE_32X32, 1, 0, 192, 50); 
	
	// Flips for sprites 1 to 3 (lol, 1, 2, 3)
	PA_SetSpriteHflip(0, 1, 1); // (screen, sprite, flip(1)/unflip(0)) HFlip -> Horizontal flip
	PA_SetSpriteVflip(0, 2, 1); // (screen, sprite, flip(1)/unflip(0)) VFlip -> Vertical flip	
	PA_SetSpriteHflip(0, 3, 1); PA_SetSpriteVflip(0, 3, 1); // Horizontal and Vertical flips...	

	while(1) // Infinite loops
	{
	
	}
	
return 0;
}
