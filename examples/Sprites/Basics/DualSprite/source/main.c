//Move a sprite using the keys...

#include <PA9.h>

// PAGfx Include
#include "all_gfx.h"
s32 x = 0;    s32 y = 0; // sprite position...

//Fonction principale du code
int main(void){

	PA_Init(); //Initialision of PAlib
	
	PA_LoadDefaultText(0,0);
	
	PA_DualLoadSpritePal(0, // Palette number
						(void*)sprite0_Pal);	// Palette name
	
	//Create the sprite
	PA_DualCreateSprite(0,(void*)vaisseau_Sprite, OBJ_SIZE_32X32,1, 0, 0, 0); // No need to choose the screen
	
	PA_OutputSimpleText(0, 2, 8, "Arrows to move the sprite and make it come on the other screen...");
	
	while(1){ // Main loop
		
		// Update the position according to the keypad...
		x += Pad.Held.Right - Pad.Held.Left;
		y += Pad.Held.Down - Pad.Held.Up;
		
		// Set the sprite's position
		PA_DualSetSpriteXY(0, // sprite
							   x, // x position
							   y); // y...
		
		PA_WaitForVBL();
	
	}
	return 0;
}