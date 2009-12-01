#include <PA9.h>

// PAGfx Include
#include "all_gfx.h"

int main(void){

	PA_Init(); //PAlib inits
	
	PA_LoadSprite16cPal(0, // Screen
					0, // Palette number
					(void*)sprite0_Pal);	// Palette name
					
	PA_CreateSprite(0, // Screen
					0, // Sprite number
					(void*)vaisseau_Sprite, // Sprite name
					OBJ_SIZE_32X32, // Sprite size
					0, // 16 color mode
					0, // Sprite palette number
					50, 50); // X and Y position on the screen
				

	while(1) // Infinite loops
	{
	
	}
	
return 0;
}