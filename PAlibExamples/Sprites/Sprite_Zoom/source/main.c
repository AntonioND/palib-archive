// Activate sprite zoom and make your sprite zoom in and out !
// Dualis is a little buggy on this for the sprite without double size...

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
	PA_SetSpriteDblsize(0, 0, 1); // Enable double size, which means that the sprite can become bigger than it's normal size
	
	// Let's do a sprit without double size to show the difference...
	PA_CreateSprite(0, 1,(void*)vaisseau_Sprite, OBJ_SIZE_32X32,1, 0, 120, 66);
	
	// Activate rotations for that sprite
	PA_SetSpriteRotEnable(0,// screen
						0,// sprite number
						0);// rotset number. You have 32 rotsets (0-31) per screen. 2 sprites with the same rotset will
							// be zoomed/rotated the same way...
	PA_SetSpriteRotEnable(0, 1, 0); // Same rotset as the other sprite... it'll be zoomed the same way
	
	u16 zoom = 256; // Zoom. 256 means no zoom, 512 is twice as small, 128 is twice as big....
	
	while(1)
	{
		zoom -= Pad.Held.Up - Pad.Held.Down; // Change the zoom according to the keys...
	
		// Fast function for zoom without rotations...
		PA_SetRotsetNoAngle(0, //screen
							0, // rotset
							zoom, zoom); // Horizontal zoom, vertical zoom. You can have a sprite streched out if you want,
							// by putting the zoom only for x or y axis....
		
		PA_WaitForVBL(); // Synch
	}
	
	return 0;
}
