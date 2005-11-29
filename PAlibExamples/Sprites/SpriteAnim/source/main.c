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
int main(void)

{
// PAlib init
PA_Init();
PA_InitVBL();

// Load the sprite palette, 
	PA_LoadSpritePal(0, // Screen
					0, // Palette number
					(void*)sprite0_Pal);	// Palette name

// Here, we'll load a sprite to animate...
PA_CreateSprite(0, 0,(void*)SpaceShipAnim_Sprite, OBJ_SIZE_64X64,1, 0, 64, 64);




while(1)
{
	++framecount;
	if (framecount == 4) framecount = 0; // There are 4 different frames, return to 0 when at the last frame...

	PA_SetSpriteAnim(0, // Screen
					0, // Sprite to animate
					framecount); // and the frame to set !
	// Note that there's a 	PA_SetSpriteAnimEx function. It works the same, but is faster, because
	// you input yourself the sprite dimensions and color mode.
	// PA_SetSpriteAnimEx(screen, sprite, lx, ly, color_mode, anim);
					
	
PA_WaitForVBL();


}
return 0;
}
