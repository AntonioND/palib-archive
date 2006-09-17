//Move a sprite using the keys...

#include <PA9.h>
#include "gfx/vaisseau.raw.c" // include the ship gfx
#include "gfx/master.pal.c"

s32 x = 0;    s32 y = 0; // sprite position...

//Fonction principale du code
int main(void){

	PA_Init(); //Initialision of PAlib
	PA_InitVBL();
	
	PA_InitText(0,0);
	
	PA_LoadPal(PAL_SPRITE0, master_Palette); // Load the sprite palette
	
	//Create the sprite
	PA_CreateSprite(0, 0,(void*)vaisseau_Bitmap, OBJ_SIZE_32X32,1, 0, 0, 0);
	
	while(1){ // Main loop
		
		// Update the position according to the keypad...
		x += Pad.Held.Right - Pad.Held.Left;
		y += Pad.Held.Down - Pad.Held.Up;
		
		// Set the sprite's position
		PA_SetSpriteXY(0, // screen
					   0, // sprite
					   x, // x position
					   y); // y...
		
		PA_WaitForVBL();
	
	}
	return 0;
}


