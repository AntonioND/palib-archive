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
	
	PA_Init3D(); // Uses Bg0, Init 3D...
	PA_Reset3DSprites(); // Init or Reset 3D Sprites
	
	
	// Initialise the text system on the top screen
	PA_InitText(1, 1);  // Initialise the text system on the top screen	
						
	PA_OutputSimpleText(1, 2, 6, "Click to set random texture");	
	
	// First, create the gfx with the corresponding images and sizes. Images converted as 256colors textures in PAGfx
	gfx[0] = PA_3DCreateTex((void*)mollusk_Texture,  // Texture
									64, 64,						// Width, Height
									TEX_256COL );				// Texture Format
	gfx[1] = PA_3DCreateTex((void*)AnarX_Texture, 64, 64, TEX_256COL);	
	gfx[2] = PA_3DCreateTex((void*)Abrexxes_Texture, 64, 64, TEX_256COL);
	
	// Load the Palettes !
	PA_Load3DSpritePal(0, // Slot
							(void*)mollusk_Pal); // Palette
	PA_Load3DSpritePal(1, (void*)AnarX_Pal);		
	PA_Load3DSpritePal(2, (void*)Abrexxes_Pal);	
		
	// Create 3 sprites...
	PA_3DCreateSpriteFromTex(0, 					// Sprite number
							gfx[0], 				// Gfx...
							64, 64, 				// Width, Height
							0,   		// Palette, unused in 16bit...
							32, 32); // X, Y SPRITE CENTER !
	PA_3DCreateSpriteFromTex(1, gfx[1], 	64, 64, 	1, 	96, 96);
	PA_3DCreateSpriteFromTex(2, gfx[2], 	64, 64, 	2, 	160, 96+64);

	s32 i;

	while(1) {
		if(Stylus.Newpress){
		   for(i = 0; i < 3; i++){
		   	if(PA_3DSpriteTouched(i)){  // If sprite touched
		   		u8 random = PA_Rand()%3;
		   	   PA_3DSetSpriteTex(i, gfx[random]); // Change to random texture...
		   	   PA_3DSetSpritePal(i, random); // Don't forget to change palette too ^^
		   	}
			}
		}						   

		PA_WaitForVBL();
		PA_3DProcess();  // Update sprites
	}


	
	return 0;
} // End of main()
