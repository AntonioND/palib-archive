/* 
//	3DCreateSprite
*/


// Includes
#include <PA9.h>       // Include for PA_Lib

#include "gfx/all_gfx.c"


u16 gfx[4];


// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	PA_Init3D(); // Uses Bg0, Init 3D...
	PA_Reset3DSprites(); // Init or Reset 3D Sprites
	
	
	// Initialise the text system on the top screen
	PA_InitText(1, 1);  // Initialise the text system on the top screen	
						
	PA_OutputSimpleText(1, 2, 2, "3DSprites Example : Alpha");	
	
	// First, create the gfx with the corresponding images and sizes. Images converted as 256colors textures in PAGfx
	gfx[0] = PA_3DCreateTex((void*)mollusk_Texture,  // Texture
									64, 64,						// Width, Height
									TEX_256COL );				// Texture Format
	gfx[1] = PA_3DCreateTex((void*)AnarX_Texture, 64, 64, TEX_256COL);	
	gfx[2] = PA_3DCreateTex((void*)Abrexxes_Texture, 64, 64, TEX_256COL);
	gfx[3] = PA_3DCreateTex((void*)pasplash_Texture, 256, 256, TEX_256COL);
		
	// Load the Palettes !
	PA_Load3DSpritePal(0, // Slot
							(void*)mollusk_Pal); // Palette
	PA_Load3DSpritePal(1, (void*)AnarX_Pal);		
	PA_Load3DSpritePal(2, (void*)Abrexxes_Pal);	
	PA_Load3DSpritePal(3, (void*)pasplash_Pal);	
			
	// Create 3 sprites...
	PA_3DCreateSpriteFromTex(0, 					// Sprite number
							gfx[0], 				// Gfx...
							64, 64, 				// Width, Height
							0,   		// Palette, unused in 16bit...
							32, 32); // X, Y SPRITE CENTER !
	PA_3DCreateSpriteFromTex(1, gfx[1], 	64, 64, 	1, 	64, 64);
	PA_3DCreateSpriteFromTex(2, gfx[2], 	64, 64, 	2, 	96, 96);
	PA_3DCreateSpriteFromTex(3, gfx[3], 	256, 256, 	3, 	128, 96);
	PA_3DSetSpritePrio(3, 800);

	// Different alpha levels for the 3 sprites
	PA_3DSetSpriteAlpha(0, 15);
	PA_3DSetSpriteAlpha(1, 10);
	PA_3DSetSpriteAlpha(2, 5);	
	PA_3DSetSpriteAlpha(2, 16);		
	
	u8 i;
	for(i = 0; i < 4; i++) PA_3DSetSpritePolyID(i, i); // Set a different polyID for each sprite (0-63 values)

	while(1) {

		PA_WaitForVBL();
		PA_3DProcess();  // Update sprites
	}


	
	return 0;
} // End of main()
