//copyright smealum http://jvteamds.free.fr

// And now, rotations and zoom at the same time, easy !

// Les Includes
#include <PA9.h>


// PAGfxConverter Include
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"

//Fonction principale du code
int main(void)

{
//Initialision de PAlib
PA_Init();
PA_InitVBL();

// Load the palette
	PA_LoadSpritePal(0, // Screen
					0, // Palette number
					(void*)sprite0_Pal);	// Palette name

// Load the sprite
PA_CreateSprite(0, 0,(void*)vaisseau_Sprite, OBJ_SIZE_32X32,1, 0, 50, 50);
PA_SetSpriteDblsize(0, 0, 1); // Enable double size, which means that the sprite can become bigger than it's normal size


// Activate rotations for that sprite
PA_SetSpriteRotEnable(0,// screen
					0,// sprite number
					0);// rotset number. You have 32 rotsets (0-31) per screen. 2 sprites with the same rotset will
						// be zoomed/rotated the same way...

u16 zoom = 256; // Zoom. 256 means no zoom, 512 is twice as small, 128 is twice as big....
u16 angle = 0; // Plain angle to start off

while(1)
{
	zoom -= Pad.Held.Up - Pad.Held.Down; // Change the zoom according to the keys...
	
	angle++;
	angle &= 511; // Limit it's range to 0-511... For other sizes, use angle %= 512, etc...
	
	// Fast function for zoom without rotations...
	PA_SetRotset(0, //screen
				0, // rotset
				angle, // Angle...
				zoom, zoom); // Horizontal zoom, vertical zoom. You can have a sprite streched out if you wany,
				// by putting the zoom only for x or y axis....
	
	PA_WaitForVBL(); // Synch

//La boucle infinie du programme
}
return 0;
}
