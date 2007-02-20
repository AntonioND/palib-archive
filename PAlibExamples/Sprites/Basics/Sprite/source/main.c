#include <PA9.h>

// PAGfx Include
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"

int main(void){

	PA_Init(); //PAlib inits
	PA_InitVBL();
		
	PA_LoadSpritePal(0, // Screen
					0, // Palette number
					(void*)sprite0_Pal);	// Palette name
			
	PA_CreateSprite(0, // Screen
					0, // Sprite number
					(void*)vaisseau_Sprite, // Sprite name
					OBJ_SIZE_32X32, // Sprite size
					1, // 256 color mode
					0, // Sprite palette number
					50, 50); // X and Y position on the screen
				

	while(1) // Infinite loops
	{
	
	}
	
return 0;
}
