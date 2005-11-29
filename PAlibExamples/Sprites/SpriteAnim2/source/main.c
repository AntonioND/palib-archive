/* This will show the new sprite animation functions !! Yay !
Thanks to http://www.reinerstileset.4players.de:1059/englisch.htm and Arog for the sprite ;)

This second edition demonstrates how to change the animation speed
*/


// Includes, only one sprite
#include <PA9.h>
// PAGfxConverter Include
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"


s16 framecount[4]; //Frame count for 4 different sprites...


// Main function
int main(void)

{
// PAlib init
PA_Init();
PA_InitVBL();

// Load the sprite palette
	PA_LoadSpritePal(0, // Screen
					0, // Palette number
					(void*)sprite0_Pal);	// Palette name


// Load a few sprites... We'll then animate them at different speeds...
u8 i = 0;
for (i = 0; i < 4; i++) PA_CreateSprite(0, i,(void*)SpaceShipAnim_Sprite, OBJ_SIZE_64X64,1, 0, i << 6, 60);




while(1)
{


for (i = 0; i < 4; i++){ // Animate the 4 sprites... It would be simplier with only one, but what the heck !
	// Each sprite will have a different speed, corresponding to i, it's number...
	
	++ framecount[i]; // The sprite's animation counter goes up by one...
	if (framecount[i] >= (4 << i)) framecount[i] = 0; // If you get to the last animation number, go back to 0...
	// << i means *2^i...
	
	PA_SetSpriteAnim(0, // Screen
					i, // Sprite to animate
					(framecount[i]>>i)); // Why divide by 2^i ? Because that way, the first sprite has animations from 
	// 0 to 3, which is fast, the second sprite ranges from 0 to 7 (O&1 do 0, 2&3 do 1, etc...), and the last ranges from 
	// 0 to 31.
	// In the end, the first sprite has a rapid animation, the last a slow animation ! Here, i is the speed !
	
}


PA_WaitForVBL();
//La boucle infinie du programme
}
return 0;
}
