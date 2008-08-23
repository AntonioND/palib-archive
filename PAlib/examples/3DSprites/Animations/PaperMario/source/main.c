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
						
	PA_OutputSimpleText(1, 0, 6, "Move the Stylus to move Mario");	
	
	// First, create the gfx with the corresponding images and sizes. Images converted as 16bit sprites in PAGfx
	gfx[0] = PA_3DCreateTex((void*)mario_Texture, 64, 64, TEX_256COL);	
	
	PA_Load3DSpritePal(0, (void*)mario_Pal);
	
	PA_EasyBgLoad(0, 3, mariobg); // Mario background
	
	// Create sprite...
	PA_3DCreateSpriteFromTex(0, 					// Sprite number
							gfx[0], 				// Gfx...
							64, 64, 				// Width, Height
							0,			 // Palette
							128, 128); // X, Y SPRITE CENTER !
							
	PA_3DSetSpriteWidthHeight(0, 96, 96);

	s16 xangle = 0;
	s16 x = 128;

	while(1) {
	   
	   if(!Stylus.Held) {
	      while(PA_3DGetSpriteAnimFrame(0) != 0){ // Wait for animation to end...
				PA_WaitForVBL();
        PA_3DProcess();  // Update sprites
		   }
			PA_3DStopSpriteAnim(0);
		}		
		
		if(Stylus.Held) {
		   if(sprite3danims[0].play == 0) PA_3DStartSpriteAnim(0, 0, 3, 10); // Sprite animation !
		   if(Stylus.X < x-12){
		      while(xangle < 256) {
				   xangle += 16; // change angle...
				   PA_3DSetSpriteRotateY(0, xangle);
					PA_WaitForVBL();
				  PA_3DProcess();  // Update sprites
				}			
		      x -= 2;
		   }
			else if(Stylus.X > x+12){
		      while(xangle > 0) {
				   xangle -= 16; // change angle...
				   PA_3DSetSpriteRotateY(0, xangle);
					PA_WaitForVBL();
				  PA_3DProcess();  // Update sprites
				}	
			   x += 2;
			}   
		   PA_3DSetSpriteX(0, x);
		}   

		PA_WaitForVBL();
		PA_3DProcess();  // Update sprites
	}


	
	return 0;
} // End of main()
