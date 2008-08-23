/* 
//	3DCreateSprite
*/


// Includes
#include <PA9.h>       // Include for PA_Lib

#include "gfx/all_gfx.c"





// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	PA_Init3D(); // Uses Bg0, Init 3D...
	PA_Reset3DSprites(); // Init or Reset 3D Sprites
	
	
	// Initialise the text system on the top screen
	PA_InitText(1, 1);  // Initialise the text system on the top screen	
						
	PA_OutputSimpleText(1, 0, 8, "Touch to deform ^^ Doesn't look as good as expected though");	
	

	// Create 3 sprites...
	PA_3DCreateSprite(0, (void*)mollusk0_Texture,64, 64, TEX_16BITS, 0, 128-32, 96-32); // X, Y SPRITE CENTER !
	PA_3DCreateSprite(1, (void*)mollusk1_Texture,64, 64, TEX_16BITS, 0, 128+32, 96-32); // X, Y SPRITE CENTER !
	PA_3DCreateSprite(2, (void*)mollusk2_Texture,64, 64, TEX_16BITS, 0, 128-32, 96+32); // X, Y SPRITE CENTER !
	PA_3DCreateSprite(3, (void*)mollusk3_Texture,64, 64, TEX_16BITS, 0, 128+32, 96+32); // X, Y SPRITE CENTER !



	while(1) {
		if((Stylus.Held)) { // in the center part
		   PA_3DSetSpriteBottomRight(0, Stylus.X - 128, Stylus.Y - 96);	
		   PA_3DSetSpriteBottomLeft(1, Stylus.X - 128, Stylus.Y - 96);				
		   PA_3DSetSpriteTopRight(2, Stylus.X - 128, Stylus.Y - 96);				
		   PA_3DSetSpriteTopLeft(3, Stylus.X - 128, Stylus.Y - 96);												   
		}

		PA_WaitForVBL();
		PA_3DProcess();  // Update sprites
	}


	
	return 0;
} // End of main()
