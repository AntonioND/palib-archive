
// Les Includes
#include <PA9.h>
#include "gfx/vaisseau.raw.c"
#include "gfx/master.pal.c"

//Fonction principale du code
int main(void)	{

	//Initialision de PAlib
	PA_Init();
	PA_InitVBL();
	
	// Load de sprite palette
	PA_LoadPal(PAL_SPRITE0, master_Palette);
	
	// Load a few sprites...
	u8 i = 0; 
	for (i = 0; i < 16; i++) PA_CreateSprite(0, i,(void*)vaisseau_Bitmap, OBJ_SIZE_32X32,1, 0, i << 4, i << 3);
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
