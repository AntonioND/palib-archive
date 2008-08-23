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
	
	PA_Init3D(); // Uses Bg0, Init 3D...
	PA_Reset3DSprites(); // Init or Reset 3D Sprites
		
	
	PA_InitText(1, 0);
	
	// Load the sprite palette, 
	PA_Load3DSpritePal(0, // Palette number
					(void*)explosion_Pal);	// Palette name
	
	// Here, we'll load a few similar sprites sprite to animate... at different speed
	PA_3DCreateSprite(0,(void*)explosion_Texture, 64, 64, TEX_256COL, 0, 32, 96);
	PA_3DCreateSprite(1,(void*)explosion_Texture, 64, 64, TEX_256COL, 0, 64+32, 96);	
	PA_3DCreateSprite(2,(void*)explosion_Texture, 64, 64, TEX_256COL, 0, 128+32, 96);	
	PA_3DCreateSprite(3,(void*)explosion_Texture, 64, 64, TEX_256COL, 0, 196+32, 96);		


	// Start the animation. Once started, it works on its own !
	PA_3DStartSpriteAnim(0, // sprite number
								0, // first frame is 0
								6, // last frame is 3, since we have 4 frames...
								5); // Speed, set to 5 frames per second						
	PA_3DStartSpriteAnim(1, 0, 6, 15); // for the second one, speed of 15 fps...	
	PA_3DStartSpriteAnim(2, 0, 6, 30); // for the third one, speed of 30 fps...	
	PA_3DStartSpriteAnim(3, 0, 6, 60); // for the last one, speed of 60 fps...

	

	while(1)
	{

		PA_WaitForVBL();
	   PA_3DProcess(); // Update...
	}
	
	return 0;
}
