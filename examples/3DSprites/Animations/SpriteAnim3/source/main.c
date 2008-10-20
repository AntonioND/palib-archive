/* This will show the new sprite animation functions !! Yay !
Thanks to http://www.reinerstileset.4players.de:1059/englisch.htm and Arog for the sprite ;)
*/


// Includes, only one sprite
#include <PA9.h>


// PAGfxConverter Include
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"


s16 framecount; //Frame count...
const unsigned char explosion[28672] __attribute__ ((aligned (4)));

// Main function
int main(void)	{
	// PAlib init
	PA_Init();
	PA_InitVBL();
	
	PA_Init3D();
	PA_Reset3DSprites();
	
	PA_InitText(0, 1);
	
	// Load the sprite palette, 
	PA_Load3DSpritePal(0, // Palette number
							(void*)explosion_Pal);	// Palette name

	// Here, we'll load a few similar sprites sprite to animate... at different speed
	PA_3DCreateSprite(0,(void*)explosion_Texture, 64, 64, TEX_256COL, 0, 32, 96);
	PA_3DCreateSprite(1,(void*)explosion_Texture, 64, 64, TEX_256COL, 0, 96, 96);	
	PA_3DCreateSprite(2,(void*)explosion_Texture, 64, 64, TEX_256COL, 0, 160, 96);	
	PA_3DCreateSprite(3,(void*)explosion_Texture, 64, 64, TEX_256COL, 0, 224, 96);		
	
	// First animation will be normal
	PA_3DStartSpriteAnim(0, // sprite number
							0, // first frame is 0
							6, // last frame is 3, since we have 4 frames...
							5); // Speed, set to 5 frames per second		
							
	// Extended animations for the rest									
	PA_3DStartSpriteAnimEx(1, 0, 6, 5, ANIM_ONESHOT); // just play it once...
	PA_3DStartSpriteAnimEx(2, 0, 6, 5, ANIM_UPDOWN, -1); // back and forth, infinite number of times	
	PA_3DStartSpriteAnimEx(3, 0, 6, 5, ANIM_LOOP, 5); // Play it 5 times

	PA_OutputSimpleText(0, 0, 6, "Infinite");
	PA_OutputSimpleText(0, 8, 7, "OneShot");	
	PA_OutputSimpleText(0, 16, 6, " UpDown");
	PA_OutputSimpleText(0, 24, 7, "5 Times");	
	
	while(1)
	{
		PA_WaitForVBL();
	  PA_3DProcess();
	}
	
	return 0;
}


