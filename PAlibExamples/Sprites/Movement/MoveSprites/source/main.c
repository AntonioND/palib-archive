
// Les Includes
#include <PA9.h>

// PAGfx Include
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"

//Fonction principale du code
int main(void)	{

	//Initialision de PAlib
	PA_Init();
	PA_InitVBL();
	
	// Load de sprite palette
	PA_LoadSpritePal(0, // Screen
					0, // Palette number
					(void*)sprite0_Pal);	// Palette name
	
	// Load a few sprites...
	u8 i = 0; 
	for (i = 0; i < 16; i++) PA_CreateSprite(0, i,(void*)vaisseau_Sprite, OBJ_SIZE_32X32,1, 0, i << 4, i << 3);
	// This loads sprites a bit everywhere
	
	
	while(1)
	{
		// Use the MoveSprite function on all sprites...
		for (i = 0; i < 16; i++) PA_MoveSprite(i);
		// The MoveSprite function checks if you are touching a sprite, and moves it around if you are... Pretty nice if you have multiple sprites around
	
		PA_WaitForVBL();
	}
	
	return 0;
}
