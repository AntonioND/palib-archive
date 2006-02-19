/* This is a very simple demo, just tells you which sprite you are touching !
*/


// Includes
#include <PA9.h>
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"

u16 pal[256]; // temporary palette to use


// Main function
int main(void)	{

	PA_Init();
	PA_InitVBL();
	
	PA_InitText(1, 0);
	
	PA_LoadSpritePal(0, 0, (void*)sprite0_Pal);	// Load the sprite palette
	
	// Load a few sprites...
	u8 i = 0; 
	for (i = 0; i < 8; i++) PA_CreateSprite(0, i,(void*)mollusk_Sprite, OBJ_SIZE_32X32,1, 0, i << 5, i << 4);
	
	PA_OutputSimpleText(1, 0, 10, "Please touch a sprite");
	
	
	s8 level = 0; // level 0 for normal, 31 would be all white, and -31 all black
	
	while(1)
	{	
	   level += Pad.Newpress.Up - Pad.Newpress.Down;   // Adjust level to the pad presses
	   PA_CreatePalBright((u16*)sprite0_Pal, pal, level);   // Create the new palette to load with the custom brightness
	   PA_LoadSpritePal(0, 0, (void*)pal);	// Load the sprite palette   // Load it !
	
		PA_WaitForVBL();
	}
	return 0;
}
