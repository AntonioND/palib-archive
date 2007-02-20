// Use 16 bit sprites ! No palettes needed for them, but they use twice as much memory...

#include <PA9.h>
#include "gfx/all_gfx.h"
#include "gfx/all_gfx.c"



//Main...
int main(void){

	PA_Init(); //PAlib Init...
	PA_InitVBL(); //VBL Init...
	
	
	// No palettes !
	
	PA_Create16bitSprite(0, // Screen
						0,	// Sprite number
						(void*)ship_Sprite, // image to load, 128 pixels wide !
						OBJ_SIZE_32X32, // sprite size
						0, // x
						50);	//y
						
	// Enable the alpha-blending
	PA_EnableSpecialFx(0, // Screen
						SFX_ALPHA, // Alpha blending mode
						0, // Nothing
						SFX_BG0 | SFX_BG1 | SFX_BG2 | SFX_BG3 | SFX_BD); // Everything normal

	PA_InitText(1, 0);
	PA_OutputText(1, 1, 12, "Press Up/Down to change alpha");

						
	u8 alpha = 15;
													
	while(1){ // Infinite loop
		alpha += Pad.Newpress.Up - Pad.Newpress.Down;
		alpha &= 15;
		
		PA_OutputText(1, 10, 14, "Alpha : %02d", alpha);
		
		PA_Set16bitSpriteAlpha(0, 0, alpha);
		PA_WaitForVBL();	
	}
	return 0;
}
