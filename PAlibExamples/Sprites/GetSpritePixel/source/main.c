#include <PA9.h>

// PAGfx Include
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"



int main(void){

	PA_Init(); //PAlib inits
	PA_InitVBL();
		
	PA_InitText(0, 0);
	PA_InitText(1, 0);
	
	
	PA_LoadSpritePal(0, // Screen
					0, // Palette number
					(void*)vaisseau_Pal);	// Palette name
					
	PA_LoadSprite16cPal(1, // Screen
					0, // Palette number
					(void*)vaisseau2_Pal);	// Palette name					
					
	PA_CreateSprite(0, // Screen
					0, // Sprite number
					(void*)vaisseau_Sprite, // Sprite name
					OBJ_SIZE_32X32, // Sprite size
					1, // 256 color mode
					0, // Sprite palette number
					0, 0); // X and Y position on the screen
					
	PA_CreateSprite(1, // Screen
					0, // Sprite number
					(void*)vaisseau2_Sprite, // Sprite name
					OBJ_SIZE_32X32, // Sprite size
					0, // 16 color mode
					0, // Sprite palette number
					0, 0); // X and Y position on the screen					
		
	PA_InitAllSpriteDraw(); // Inits the info to draw on sprite or retrieve pixel colors		

	while(1) // Infinite loops
	{
	   if((Stylus.X < 32)&&(Stylus.Y < 32)){  // If in sprite zone, check pixel color for both sprites
	      PA_OutputText(0, 10, 2, "Pixel : %03d", PA_GetSpritePixel(0, 0, Stylus.X, Stylus.Y));
	      PA_OutputText(1, 10, 2, "Pixel : %03d", PA_GetSprite16cPixel(1, 0, Stylus.X, Stylus.Y));	      
	      
	   }  
		PA_WaitForVBL();
	}
	
return 0;
}
