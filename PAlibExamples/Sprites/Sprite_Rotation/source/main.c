// Activate sprite rotations and make your sprite turn around !

// Les Includes
#include <PA9.h>
#include "gfx/vaisseau.raw.c"
#include "gfx/master.pal.c"

//Fonction principale du code
int main(void)

{
//Initialision de PAlib
PA_Init();
PA_InitVBL();

// Load the palette
PA_LoadPal(PAL_SPRITE0, master_Palette);

// Load the sprite
PA_CreateSprite(0, 0,(void*)vaisseau_Bitmap, OBJ_SIZE_32X32,1, 0, 50, 50);

// Activate rotations for that sprite
PA_SetSpriteRotEnable(0,// screen
					0,// sprite number
					0);// rotset number. You have 32 rotsets (0-31) per screen. 2 sprites with the same rotset will
						// be zoomed/rotated the same way...

u16 angle = 0; // Rotation angle...

while(1)
{
	++angle; // change the angle
	angle &= 511; // limit the range to 0-511. works only with 1, 3, 7, 15, 31, etc... (2^n  - 1)
	
	// Fast function for rotations without zoom...
	PA_SetRotsetNoZoom(0, //screen
						0, // rotset
						angle); // angle, from 0 to 511
	
	PA_WaitForVBL(); // Synch

//La boucle infinie du programme
}
return 0;
}
