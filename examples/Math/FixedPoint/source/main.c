#include <PA9.h>

// PAGfxConverter Include
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"


int main(void){

	PA_Init(); //PAlib inits
	PA_InitVBL();
	
	PA_InitText(0, 0);
	
	s32 speed1 = 256;  // first speed...0
	PA_OutputText(0, 15, 2,  " 1   pixel/frame");
	
	s32 speed2 = 128;	
	PA_OutputText(0, 15, 10, "0.5  pixel/frame");
	
	s32 speed3 = 64;		
	PA_OutputText(0, 15, 18, "0.25 pixel/frame");	
	
	PA_LoadSpritePal(0, 0, (void*)sprite0_Pal);	// Palette name
					
	PA_CreateSprite(0, 0, (void*)vaisseau_Sprite, OBJ_SIZE_32X32, 1, 0, 0, 0); 
	PA_CreateSprite(0, 1, (void*)vaisseau_Sprite, OBJ_SIZE_32X32, 1, 0, 0, 64); 					
	PA_CreateSprite(0, 2, (void*)vaisseau_Sprite, OBJ_SIZE_32X32, 1, 0, 0, 128); 
	
	// all sprites stick to the left
	s32 spritex1 = 0;	s32 spritex2 = 0;	s32 spritex3 = 0;
	
		
	while(1) // Infinite loops
	{
		// Move all the sprites by their corresponding speed
		spritex1 += speed1; spritex2 += speed2; spritex3 += speed3; 
		
		// Positionne all the sprites, >>8 to return to normal position
		PA_SetSpriteX(0, 0, spritex1>>8);
		PA_SetSpriteX(0, 1, spritex2>>8);		
		PA_SetSpriteX(0, 2, spritex3>>8);		
			
		PA_WaitForVBL();
	}
	
return 0;
}
