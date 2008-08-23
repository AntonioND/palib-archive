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
	
	// Create 4 sprites for the different flips......
	PA_3DCreateSpriteFromTex(0, gfx[0], 64, 64, 	0,		80, 48); // X, Y SPRITE CENTER !
	PA_3DCreateSpriteFromTex(1, gfx[0], 64, 64, 	0,		80+96, 48); // X, Y SPRITE CENTER !	
	PA_3DCreateSpriteFromTex(2, gfx[0], 64, 64, 	0,		80, 80+64); // X, Y SPRITE CENTER !
	PA_3DCreateSpriteFromTex(3, gfx[0], 64, 64, 	0,		80+96, 80+64); // X, Y SPRITE CENTER !
	
	// Set the flips
	PA_3DSetSpriteHflip(0, 0);		PA_3DSetSpriteVflip(0, 0);
	PA_3DSetSpriteHflip(1, 1);		PA_3DSetSpriteVflip(1, 0);	
	PA_3DSetSpriteHflip(2, 0);		PA_3DSetSpriteVflip(2, 1);
	PA_3DSetSpriteHflip(3, 1);		PA_3DSetSpriteVflip(3, 1);	
		
	while(1) {


		PA_WaitForVBL();
		PA_3DProcess();  // Update sprites
	}


	
	return 0;
} // End of main()
