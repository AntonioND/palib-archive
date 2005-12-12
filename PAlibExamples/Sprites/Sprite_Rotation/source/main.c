// Activate sprite rotations and make your sprite turn around !

#include <PA9.h>

// PAGfxConverter Include
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"


int main(void){
	
	PA_Init();
	PA_InitVBL();
	
	// Load the palette
		PA_LoadSpritePal(0, // Screen
						0, // Palette number
						(void*)sprite0_Pal);	// Palette name
	
	// Load the sprite
	PA_CreateSprite(0, 0,(void*)vaisseau_Sprite, OBJ_SIZE_32X32,1, 0, 50, 50);
	
	// Activate rotations for that sprite
	PA_SetSpriteRotEnable(0,// screen
						0,// sprite number
						0);// rotset number. You have 32 rotsets (0-31) per screen. 2 sprites with the same rotset will
							// be zoomed/rotated the same way...
	
	u16 angle = 0; // Rotation angle...
	
	while(1)
	{
		++angle; // change the angle
		angle &= 511; // limit the range to 0-511. works only with 1, 3, 7, 15, 31, etc... (2^n  - 1)
		
		// Fast function for rotations without zoom...
		PA_SetRotsetNoZoom(0, //screen
							0, // rotset
							angle); // angle, from 0 to 511
		
		PA_WaitForVBL(); // Synch
	}
	
	return 0;
}
