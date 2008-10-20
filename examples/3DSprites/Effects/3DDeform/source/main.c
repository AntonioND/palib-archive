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
						
	PA_OutputSimpleText(1, 0, 6, "Touch one of the screen corners to move the sprite corners...");	
	

	// Create 3 sprites...
	PA_3DCreateSprite(0, 					// Sprite number
							(void*)mollusk_Texture,
							64, 64, 				// Width, Height
							TEX_16BITS,
							0,   		// Palette, unused in 16bit...
							128, 96); // X, Y SPRITE CENTER !


	while(1) {
		if(Stylus.Held) {
		   if((Stylus.X < 128)&&(Stylus.Y < 96)) PA_3DSetSpriteTopLeft(0, Stylus.X - (128-32), Stylus.Y - (96-32));	 // Top left corner change
		   if((Stylus.X > 128)&&(Stylus.Y < 96)) PA_3DSetSpriteTopRight(0, Stylus.X - (128+32), Stylus.Y - (96-32));	 // Top left corner change
		   if((Stylus.X < 128)&&(Stylus.Y > 96)) PA_3DSetSpriteBottomLeft(0, Stylus.X - (128-32), Stylus.Y - (96+32));	 // Top left corner change
		   if((Stylus.X > 128)&&(Stylus.Y > 96)) PA_3DSetSpriteBottomRight(0, Stylus.X - (128+32), Stylus.Y - (96+32));	 // Top left corner change												   
		}

		PA_WaitForVBL();
		PA_3DProcess();  // Update sprites
	}


	
	return 0;
} // End of main()
