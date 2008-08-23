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
	
	PA_Init3D(); // Uses Bg0
	PA_Reset3DSprites();
	
	// Initialise the text system on the top screen
	PA_InitText(0, 1);
	PA_InitText(1, 1);  // Initialise the text system on the top screen	
						
	PA_OutputSimpleText(1, 0, 6, "Move the Sprite with the stylus");	
	
	// First, create the gfx with the corresponding images and sizes. Images converted as 16bit sprites in PAGfx
	gfx[0] = PA_3DCreateTex((void*)mollusk_Texture, 64, 64, TEX_16BITS);	
	
	// Create sprite...
	PA_3DCreateSpriteFromTex(0, 					// Sprite number
							gfx[0], 				// Gfx...
							64, 64, 				// Width, Height
							0,			 // Palette
							128, 96); // X, Y SPRITE CENTER !

	while(1) {
	   
		if(Stylus.Held) PA_3DSetSpriteXY(0, Stylus.X, Stylus.Y); // Move sprite 0 to Stylus Position... This is the sprite's center !

		PA_WaitForVBL();
		PA_3DProcess();  // Update sprites
	}


	
	return 0;
} // End of main()
