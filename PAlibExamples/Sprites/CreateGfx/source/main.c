/* This simple demo will show you how to effectively use Sprite Gfxs
In a few word, when you load a sprite, it copies the sprite information (size, position, etc...)
and the sprite's image, the gfx... If you create 10 times the same sprite, you'll have 10 times
the same image in VRAM, which is kind of a shame... So let's see how to have a single copy of that image,
and thus save VRAM and loading time...
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

// Load the sprite palette, for both screens
	PA_LoadSpritePal(0, // Screen
					0, // Palette number
					(void*)master_Palette);	// Palette name
	PA_LoadSpritePal(1, // Screen
					0, // Palette number
					(void*)master_Palette);	// Palette name


// Now, let's copy the image of the sprite to VRAM :
// First screen
u16 gfx0 = PA_CreateGfx(0, // screen
			(void*)vaisseau_Bitmap, // Image
			OBJ_SIZE_32X32, // Size
			1); // 256 color sprite
// Same thing for the top screen : 
u16 gfx1 = PA_CreateGfx(1, (void*)vaisseau_Bitmap, OBJ_SIZE_32X32, 1);

// The gfx0 and gfx1 variables stock the number of the gfx, this will allow us to create sprites later on


PA_OutputSimpleText(1, 0, 10, "Press A to create a sprite on the bottom screen, B on the top screen");

u8 nsprites0 = 0; // Number of sprites on the bottom screen
u8 nsprites1 = 0; // Top screen

while(1)
{

// If A pressed
if (Pad.Newpress.A) {
	PA_CreateSpriteFromGfx(0, // screen
							nsprites0, // Next sprite to load...
							gfx0, // image to use in memory, no image copying !
							OBJ_SIZE_32X32, 1, 0, PA_Rand()%256, PA_Rand()%192); // The rest is like normal sprites
	++nsprites0; // Next time, load the next sprite number
}
// Same thing with B
if (Pad.Newpress.B) {
	PA_CreateSpriteFromGfx(1, // screen
							nsprites1, // Next sprite to load...
							gfx1, // image to use in memory, no image copying !
							OBJ_SIZE_32X32, 1, 0, PA_Rand()%256, PA_Rand()%192); // The rest is like normal sprites
	++nsprites1; // Next time, load the next sprite number
}


PA_WaitForVBL();
//La boucle infinie du programme
}
return 0;
}
