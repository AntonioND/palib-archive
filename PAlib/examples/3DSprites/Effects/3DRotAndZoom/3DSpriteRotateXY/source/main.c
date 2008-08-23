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
						
	PA_OutputSimpleText(1, 2, 6, "3D Sprite Rotations Revisited ^^");	
	
	// First, create the gfx with the corresponding images and sizes. Images converted as 16bit textures in PAGfx
	gfx[0] = PA_3DCreateTex((void*)mollusk_Texture, 64, 64, TEX_16BITS);	
	gfx[1] = PA_3DCreateTex((void*)AnarX_Texture, 64, 64, TEX_16BITS);	
	gfx[2] = PA_3DCreateTex((void*)Abrexxes_Texture, 64, 64, TEX_16BITS);	
	

	// Create 3 sprites...
	PA_3DCreateSpriteFromTex(0, 					// Sprite number
							gfx[0], 				// Gfx...
							64, 64, 				// Width, Height
							0, 		// Palette
							32, 32); // X, Y SPRITE CENTER !
	PA_3DCreateSpriteFromTex(1, gfx[1], 	64, 64, 	0, 	96, 96);
	PA_3DCreateSpriteFromTex(2, gfx[2], 	64, 64, 	0, 	160, 96+64);

	u16 rot = 0;

	while(1) {
		rot += 4; // add to angle...
		rot &= 511; // limit range to 0-511...
//		if(Stylus.Held) PA_3DSetSpriteXY(0, Stylus.X, Stylus.Y);
		
		PA_3DSetSpriteRotateX(0, rot); // Xrot
		PA_3DSetSpriteRotateY(1, rot); // Yrot	
		PA_3DSetSpriteRotateXYZ(2, rot, rot, rot); // Let's do triple rotation and see what it does ^^
		

		PA_WaitForVBL();
		PA_3DProcess();  // Update sprites
	}


	
	return 0;
} // End of main()
