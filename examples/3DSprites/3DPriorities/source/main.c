/* 
//	3DCreateSprite
*/


// Includes
#include <PA9.h>       // Include for PA_Lib

#include "gfx/all_gfx.c"


u16 gfx[3];

 



// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	PA_Init3D(); // Uses Bg0, Init 3D...
	PA_Reset3DSprites(); // Init or Reset 3D Sprites
	
	
	// Initialise the text system on the top screen
	PA_InitText(1, 1);  // Initialise the text system on the top screen	
						
	PA_OutputSimpleText(1, 0, 8, "Move 3DSprite to change priority");	
	
	// First, create the gfx with the corresponding images and sizes. Images converted as 256colors textures in PAGfx
	gfx[0] = PA_3DCreateTex((void*)mollusk_Texture,  // Texture
									64, 64,						// Width, Height
									TEX_256COL );				// Texture Format

	
	// Load the Palettes !
	PA_Load3DSpritePal(0, // Slot
							(void*)mollusk_Pal); // Palette

	// Create a few sprites
	PA_3DCreateSpriteFromTex(0, gfx[0], 	64, 64, 	0, 	128, 96);

	u8 i;
	for(i = 0; i < 24; i++){
	   PA_3DCreateSpriteFromTex(i+1, gfx[0], 	64, 64, 	0, 	32, i*8);
	   PA_3DSetSpritePrio(i+1, 1024 + i*8); // Priority... (default is 1024)
	}   

	while(1) {
		if(Stylus.Held) {
		   PA_3DSetSpriteXY(0, Stylus.X, Stylus.Y);
		   PA_3DSetSpritePrio(0, 1024 + Stylus.Y); // Priority depending on Y position...
		}   

		PA_WaitForVBL();
		PA_3DProcess();  // Update sprites
	}


	
	return 0;
} // End of main()
