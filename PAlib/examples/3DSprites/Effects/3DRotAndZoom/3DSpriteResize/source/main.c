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
						
	PA_OutputSimpleText(1, 2, 6, "Touch the screen to resize !");	
	
	// First, create the gfx with the corresponding images and sizes. Images converted as 16bit textures in PAGfx
	gfx[0] = PA_3DCreateTex((void*)mollusk_Texture, 64, 64, TEX_16BITS);	
	
	// Create sprite...
	PA_3DCreateSpriteFromTex(0, 					// Sprite number
							gfx[0], 				// Gfx...
							64, 64, 				// Width, Height
							0,			 // Palette
							128, 96); // X, Y SPRITE CENTER !

	while(1) {
		if(Stylus.Held){ // Resize if held...
			if(Stylus.X >= 128) PA_3DSetSpriteWidth(0, (Stylus.X-128)*2);
			else PA_3DSetSpriteWidth(0, (128-Stylus.X)*2);
			if(Stylus.Y >= 96) PA_3DSetSpriteHeight(0, (Stylus.Y-96)*2);
			else PA_3DSetSpriteHeight(0, (96-Stylus.Y)*2);		   
		   
		}   


		PA_WaitForVBL();
		PA_3DProcess();  // Update sprites
	}


	
	return 0;
} // End of main()
