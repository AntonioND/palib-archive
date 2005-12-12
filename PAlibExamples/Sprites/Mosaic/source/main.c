#include <PA9.h>

// PAGfxConverter Include
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"

u8 Xmosaic = 0;
u8 Ymosaic = 0;

int main(void){

    PA_Init();    // Initializes PA_Lib
    PA_InitVBL(); // Initializes a standard VBL

	PA_LoadSpritePal(0, 0, (void*)sprite0_Pal);	// Palette loading...
					
	PA_CreateSprite(0, 0, (void*)mollusk_Sprite, OBJ_SIZE_32X32, 1, 0, 0, 50); // Sprite...
	
	// Activate the mosaic effect for a given sprite !
	PA_SetSpriteMosaic(0, 0, 1); // Screen, Sprite, Mosaic on/off (1/0)


    // Infinite loop to keep the program running
    while (1)
    {
		// Change the mosaic effect according to keys...
        Xmosaic = Xmosaic + Pad.Newpress.Right - Pad.Newpress.Left;
        Ymosaic = Ymosaic + Pad.Newpress.Up - Pad.Newpress.Down;

        PA_SetSpriteMosaicXY(0, // screen
							Xmosaic, // X Mosaic value, between 0 and 15. This is the mosaic base size, 1 for 1 pixel, etc...
							Ymosaic); // Y Mosaic value, between 0 and 15. This is the mosaic base size, 1 for 1 pixel, etc...

        PA_WaitForVBL();
    }
	
return 0;
}
