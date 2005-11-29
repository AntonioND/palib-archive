/* This is a very simple demo, just tells you which sprite you are touching !
*/


// Includes, only one sprite
#include <PA9.h>
#include "gfx/vaisseau.raw.c"
#include "gfx/master.pal.c"

// Main function
int main(void)

{
// PAlib init
PA_Init();
PA_InitVBL();
PA_InitText(1, 0);

// Load the sprite palette
	PA_LoadSpritePal(0, // Screen
					0, // Palette number
					(void*)master_Palette);	// Palette name


// Load a few sprites...
u8 i = 0; 
for (i = 0; i < 8; i++) PA_CreateSprite(0, i,(void*)vaisseau_Bitmap, OBJ_SIZE_32X32,1, 0, i << 5, i << 4);


PA_OutputSimpleText(1, 0, 10, "Please touch a sprite");



while(1)
{

// Now we'll test every sprite to see if we touch it...
for (i = 0; i < 8; i++) {
	// If we touch the sprite, returns 1...
	if (PA_SpriteTouched(i)) PA_OutputText(1, 0, 15, "Sprite %d  ", i);
}


PA_WaitForVBL();
//La boucle infinie du programme
}
return 0;
}
