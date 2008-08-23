


#include <PA9.h>

// PAGfxConverter Include
#include "gfx/all_gfx.h" // Only include the .h to use the .bin files in the data folder


int main(void){

	PA_Init(); //PAlib inits
	PA_InitVBL();
	
	PA_InitText(1, 0);
	
	PA_OutputSimpleText(1, 2, 6, "BinFile example");
	
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
				

	while (1) // Infinite loop
	{
		PA_WaitForVBL();
	}
	
return 0;
}
