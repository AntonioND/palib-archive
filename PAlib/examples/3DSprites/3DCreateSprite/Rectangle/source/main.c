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
						
	PA_OutputSimpleText(1, 2, 2, "3DSprites Example : 256colors");	
	

	// Load the Palettes !
	PA_Load3DSpritePal(0, // Slot
							(void*)bar_Pal); // Palette

		
	// Create 3 sprites...
	PA_3DCreateSprite(0, 					// Sprite number
							(void*)bar_Texture, 	// Texture
							256, 32, 				// Width, Height
							TEX_256COL,			// Type
							0,   		// Palette, unused in 16bit...
							128, 96); // X, Y SPRITE CENTER !


	while(1) {


		PA_WaitForVBL();
		PA_3DProcess();  // Update sprites
	}


	
	return 0;
} // End of main()
