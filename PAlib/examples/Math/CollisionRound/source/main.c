
// Includes
#include <PA9.h>

// PAGfxConverter Include
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"

int main(void){

	PA_Init();
	PA_InitVBL();
	
	PA_InitText(1,0); // On the top screen

	PA_LoadSpritePal(0, 0, (void*)sprite0_Pal);
	
	// This'll be the movable sprite...
	PA_CreateSprite(0, 0,(void*)circle_Sprite, OBJ_SIZE_32X32,1, 0, 16, 16); 	
	s32 x = 16; s32 y = 16; // Sprite's center position
	
	// This will be the fixed circle
	PA_CreateSprite(0, 1,(void*)circle_Sprite, OBJ_SIZE_32X32,1, 0, 128-16, 96-16); 
	
	while(1)
	{
		if (PA_MoveSprite(0)){
			x = PA_MovedSprite.X;
			y = PA_MovedSprite.Y;
		}
		
		// Collision ?
		if (PA_Distance(x, y, 128, 96) < 32*32) PA_OutputText(1, 2, 10, "Collision !!");
		else PA_OutputText(1, 2, 10, "            ");

		PA_WaitForVBL();
	}
	return 0;
}
