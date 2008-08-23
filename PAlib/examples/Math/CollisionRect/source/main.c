
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
	PA_CreateSprite(0, 0,(void*)rect_Sprite, OBJ_SIZE_32X16,1, 0, 0, 0); 	
	s32 x1 = 16; s32 y1 = 8; // Sprite's center position
	s8 w1 = 32; s8 h1 = 16; // width and height...
	
	// This will be the fixed circle
	PA_CreateSprite(0, 1,(void*)rect_Sprite, OBJ_SIZE_16X32,1, 0, 128-8, 96-16); 
	s32 x2 = 128; s32 y2 = 96; // Sprite's center position
	s8 w2 = 16;  s8 h2 = 32; // width and height...	
	
	while(1)
	{
		if (PA_MoveSprite(0)){
			x1 = PA_MovedSprite.X;
			y1 = PA_MovedSprite.Y;
		}
		
		// Collision ?
		if ((x2 >= x1 - (w1 + w2)/2) && (x2 <= x1 + (w1 + w2)/2) && (y2 >= y1 - (h1 + h2)/2) && (y2 <= y1 + (h1 + h2)/2)) PA_OutputText(1, 2, 10, "Collision !!");
		else PA_OutputText(1, 2, 10, "            ");

		PA_WaitForVBL();
	}
	return 0;
}
