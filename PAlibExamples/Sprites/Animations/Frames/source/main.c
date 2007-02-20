/* Sprite Frames...
*/


// Includes, only one sprite
#include <PA9.h>


// PAGfxConverter Include
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"


s16 framecount; //Frame count...


// Main function
int main(void)	{
	// PAlib init
	PA_Init();
	PA_InitVBL();
	
	PA_InitText(1, 0);
	
	// Load the sprite palette, 
	PA_LoadSpritePal(0, // Screen
					0, // Palette number
					(void*)sprite0_Pal);	// Palette name

	PA_CreateSprite(0, 0,(void*)frames_Sprite, OBJ_SIZE_16X32,1, 0, 128-16, 64);

	while(1)
	{
		if (Pad.Held.Up) PA_SetSpriteAnim(0, 0, 0); // screen, sprite, frame
		if (Pad.Held.Down) PA_SetSpriteAnim(0, 0, 2); // screen, sprite, frame
		if (Pad.Held.Left) PA_SetSpriteAnim(0, 0, 3); // screen, sprite, frame
		if (Pad.Held.Right) PA_SetSpriteAnim(0, 0, 1); // screen, sprite, frame		
	
		PA_WaitForVBL();
	}
	
	return 0;
}
