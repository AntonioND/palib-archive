/* 
//	3DCreateSprite
*/


// Includes
#include <PA9.h>       // Include for PA_Lib

#include "Abrexxes.h"
#include "AnarX.h"
#include "mollusk.h"


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
						
	PA_OutputSimpleText(1, 2, 2, "3DSprites Example : Using Gif");	
	
	
	// Before using any gif function for the textures, set the transparency color to use !
	PA_GifToTexTransp(PA_RGB(31, 0, 31)); //  Set transparent color, Magenta in our case :)
	
	
	// First, create the gfx from the gif files
	gfx[0] = PA_3DCreateTexFromGif((void*)mollusk, 0); // gif, palette number
	gfx[1] = PA_3DCreateTexFromGif((void*)Abrexxes, 1); // gif, palette number

		
	// Create 2 sprites from the textures, as always
	PA_3DCreateSpriteFromTex(0, 					// Sprite number
							gfx[0], 				// Gfx...
							64, 64, 				// Width, Height
							0,   		// Palette, unused in 16bit...
							32, 32); // X, Y SPRITE CENTER !
	PA_3DCreateSpriteFromTex(1, gfx[1], 	64, 64, 	1, 	96, 96);
	
	// Or just load directly from the gif
	PA_3DCreateSpriteFromGif(2, 		 		// Sprite
									(void*)AnarX,  // Gif
									2, 				// Palette number
									160, 96+64);	// X, Y SPRITE CENTER !


	while(1) {


		PA_WaitForVBL();
		PA_3DProcess();  // Update sprites
	}


	
	return 0;
} // End of main()
