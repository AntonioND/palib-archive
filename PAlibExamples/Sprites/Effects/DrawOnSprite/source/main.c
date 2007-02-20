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

	PA_InitText(1, 0);

	PA_CreateSprite(0, 0, (void*)Blank, // Blank sprite !
					OBJ_SIZE_64X64, // Sprite size
					1, 0, // Sprite palette number
					0, 0); // In the upper left corner
	PA_CreateSprite(0, 1, (void*)Blank, // Blank sprite !
					OBJ_SIZE_64X64, // Sprite size
					1, 0, // Sprite palette number
					64, 0); // To the right			
					
	u8 x = 32; u8 y = 32;  // start at the center of the sprite
	PA_InitAllSpriteDraw(); // Init the draw system for all onscreen sprites
	
	
	while(1) // Infinite loops
	{
	   x+= Pad.Held.Right - Pad.Held.Left;
	   y += Pad.Held.Down - Pad.Held.Up;
	   
	   PA_SetSpritePixel(0, 0, x, y, 10); // draw on sprite 0 on the bottom screen, color 10
	   PA_SetSpritePixel(0, 1, x, y, 10);	 // draw on sprite 1 on the bottom screen, color 10  
	   
	   PA_OutputText(1, 2, 10, "%02d, %02d    ", x, y);
	   
	   PA_WaitForVBL();
	   
	
	}
	
return 0;
}
