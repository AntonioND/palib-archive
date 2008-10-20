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
	PA_InitText(0, 1);  	
								
	PA_OutputSimpleText(1, 2, 2, "3DSprites Example : 256colors");	
	
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


	while(1) {

		// Check if you touch one of them...
		if(Stylus.Held){
			if(PA_3DSpriteTouched(0)) PA_OutputSimpleText(0, 10, 2, "Mollusk is touched ! ");
			else if(PA_3DSpriteTouched(1)) PA_OutputSimpleText(0, 10, 2, "AnarX is touched !  ");
			else if(PA_3DSpriteTouched(2)) PA_OutputSimpleText(0, 10, 2, "Abrexxes is touched !");		
			else PA_OutputSimpleText(0, 10, 2, "                         ");	 // If not, erase ^^	
		}			
		else PA_OutputSimpleText(0, 10, 2, "                         ");	 // If not, erase ^^

		PA_WaitForVBL();
		PA_3DProcess();  // Update sprites
	}


	
	return 0;
} // End of main()
