/* This will show the new sprite animation functions !! Yay !
Thanks to http://www.reinerstileset.4players.de:1059/englisch.htm and Arog for the sprite ;)
*/


// Includes, only one sprite
#include <PA9.h>


// PAGfxConverter Include
#include "all_gfx.h"

s16 framecount; //Frame count...


// Main function
int main(void)	{
	// PAlib init
	PA_Init();
	
	PA_LoadDefaultText(1, 0);

	PA_LoadSpritePal(0, 0, (void*)sprite0_Pal);	// Palette....	

	s32 x = 120; s32 y = 64;

	PA_CreateSprite(0, 0,(void*)som_Sprite, OBJ_SIZE_16X32,1, 0, x, y); // Sprite
	
	while(1)
	{
		// Animation code...
		if(Pad.Newpress.Up) PA_StartSpriteAnim(0, 0, 0, 3, 4);
		if(Pad.Newpress.Down) PA_StartSpriteAnim(0, 0, 8, 11, 4);		
		
		if(Pad.Newpress.Right) {
			PA_StartSpriteAnim(0, 0, 4, 7, 4);	
			PA_SetSpriteHflip(0, 0, 0);
		}
		if(Pad.Newpress.Left) {
			PA_StartSpriteAnim(0, 0, 4, 7, 4);	
			PA_SetSpriteHflip(0, 0, 1);
		}

		
		if(!((Pad.Held.Left)||(Pad.Held.Up)||(Pad.Held.Down)||(Pad.Held.Right))) PA_SpriteAnimPause(0, 0, 1);
	
	
		// Moving Code
		y += Pad.Held.Down - Pad.Held.Up;
		x += Pad.Held.Right - Pad.Held.Left;		
		PA_SetSpriteXY(0, 0, x, y);
	
		PA_WaitForVBL();
	}
	
	return 0;
}