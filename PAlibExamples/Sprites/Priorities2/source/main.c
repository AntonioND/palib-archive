/* This is a very simple demo, just tells you which sprite you are touching !
*/


// Includes
#include <PA9.h>
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"



// Main function
int main(void)	{

	PA_Init();
	PA_InitVBL();
	
	PA_InitText(1, 0);
	
	PA_LoadSpritePal(0, 0, (void*)sprite0_Pal);	// Load the sprite palette
	
	// Load a few sprites...
	u8 i = 0; 
	for (i = 0; i < 16; i++) PA_CreateSprite(0, i,(void*)mollusk_Sprite, OBJ_SIZE_32X32,1, 0, i << 3, i << 3);
	
	
	PA_InitSpriteExtPrio(1); // Enable extended priorities
	
	
	PA_OutputSimpleText(1, 0, 10, "Please move the sprites to see how their priorities change");
	
	while(1)
	{	
		// Now we'll test every sprite to see if we touch it...
		for (i = 0; i < 16; i++) {
			PA_MoveSprite(i);  // move the sprites. 
			s16 y = PA_GetSpriteY(0, i);  if (y > 192) y-=256; // if higher than 0...
			PA_SetSpriteExtPrio(0, i, 192-y); // Top priority when at the bottom !
		}

	
		PA_WaitForVBL();	
	}
	return 0;
}
