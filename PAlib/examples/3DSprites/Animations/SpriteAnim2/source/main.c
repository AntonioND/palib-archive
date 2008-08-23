/* This will show the new sprite animation functions !! Yay !
Thanks to http://www.reinerstileset.4players.de:1059/englisch.htm and Arog for the sprite ;)
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
	
	PA_Init3D();
	PA_Reset3DSprites();
	
	PA_InitText(1, 0);

	PA_Load3DSpritePal(0, (void*)som_Pal);	// Palette....	

	s32 x = 128; s32 y = 96;

	PA_3DCreateSprite(0,(void*)som_Texture, 32, 32, TEX_256COL, 0, x, y); // Sprite
	
	while(1)
	{
		// Animation code...
		if(Pad.Newpress.Up) PA_3DStartSpriteAnim(0, 0, 3, 6);
		if(Pad.Newpress.Down) PA_3DStartSpriteAnim(0, 8, 11, 6);		
		
		if(Pad.Newpress.Right) {
			PA_3DStartSpriteAnim(0, 4, 7, 6);	
			PA_3DSetSpriteHflip(0, 0);
		}
		if(Pad.Newpress.Left) {
			PA_3DStartSpriteAnim(0, 4, 7, 6);	
			PA_3DSetSpriteHflip(0, 1);
		}

		
		if(!((Pad.Held.Left)||(Pad.Held.Up)||(Pad.Held.Down)||(Pad.Held.Right))) PA_3DSpriteAnimPause(0, 1);
	
	
		// Moving Code
		y += Pad.Held.Down - Pad.Held.Up;
		x += Pad.Held.Right - Pad.Held.Left;		
		PA_3DSetSpriteXY(0, x, y);
	
		PA_WaitForVBL();
		PA_3DProcess(); // Update 3D...
	}
	
	return 0;
}
