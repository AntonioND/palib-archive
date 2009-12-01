/* 
//	3DCreateSprite
*/


// Includes
#include <PA9.h>       // Include for PA_Lib

#include "all_gfx.h"


u16 gfx[3];



// Function: main()
int main()
{
	PA_Init();    // Initializes PA_Lib
	
	PA_Init3D(); // Uses Bg0, Init 3D...
	PA_Reset3DSprites(); // Init or Reset 3D Sprites
	
	
	// Initialise the text system on the top screen
	PA_LoadDefaultText(1, 1);  // Initialise the text system on the top screen	
						
	PA_OutputSimpleText(1, 2, 6, " 3DSprites Example : 16bits");	
	
	// First, create the gfx with the corresponding images and sizes. Images converted as 16bit textures in PAGfx
	gfx[0] = PA_3DCreateTex((void*)mollusk_Texture,  // Texture
									64, 64,						// Width, Height
									TEX_16BITS );				// Texture Format
	gfx[1] = PA_3DCreateTex((void*)AnarX_Texture, 64, 64, TEX_16BITS);	
	gfx[2] = PA_3DCreateTex((void*)Abrexxes_Texture, 64, 64, TEX_16BITS);	
	
	// Create 3 sprites...
	PA_3DCreateSpriteFromTex(0, 					// Sprite number
							gfx[0], 				// Gfx...
							64, 64, 				// Width, Height
							0,   		// Palette, unused in 16bit...
							32, 32); // X, Y SPRITE CENTER !
	PA_3DCreateSpriteFromTex(1, gfx[1], 	64, 64, 	0, 	96, 96);
	PA_3DCreateSpriteFromTex(2, gfx[2], 	64, 64, 	0, 	160, 96+64);


	while(1) {


		PA_WaitForVBL();
		PA_3DProcess();  // Update sprites
	}


	
	return 0;
} // End of main()