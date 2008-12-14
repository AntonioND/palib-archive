/* Sprite Frames...
*/


// Includes, only one sprite
#include <PA9.h>


// PAGfxConverter Include
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"


s16 framecount; //Frame count...
u16 gfx[3];




// Main function
int main(void)	{
	// PAlib init
	PA_Init();
	PA_InitVBL();
	
	PA_Init3D(); // Uses Bg0, Init 3D...
	PA_Reset3DSprites(); // Init or Reset 3D Sprites
	
		
	
	PA_InitText(1, 0);
	
	PA_OutputText(1, 0, 10, "Press Pad to change frame");
	
	// Load the sprite palette, 
	PA_Load3DSpritePal(0, // Palette number
					(void*)frames_Pal);	// Palette name
	
	gfx[0] = PA_3DCreateTex((void*)frames_Texture, 32, 32, TEX_256COL);
	
	PA_3DCreateSpriteFromTex(0, gfx[0], 	32, 32, 	0, 	128, 96);
	
	
	while(1)
	{
		if (Pad.Held.Up) PA_3DSetSpriteFrame(0, 0); // screen, sprite, frame
		if (Pad.Held.Down) PA_3DSetSpriteFrame(0, 2); // screen, sprite, frame
		if (Pad.Held.Left) PA_3DSetSpriteFrame(0, 3); // screen, sprite, frame
		if (Pad.Held.Right) PA_3DSetSpriteFrame(0, 1); // screen, sprite, frame		
	
		PA_WaitForVBL();
		PA_3DProcess();
	}
	
	return 0;
}
