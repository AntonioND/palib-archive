/* 
//	3D Sprite Moving example...
*/


// Includes
#include <PA9.h>       // Include for PA_Lib

#include "gfx/all_gfx.c"

#define N_SPRITES 1024 // Number of sprites in the example

u16 gfx[3];

typedef struct{
   s32 x, y, vx, vy;
} spritepos;
spritepos sprites[1024];  // Let's manage the sprite positions ^^


  

// Function: main()
int main(void)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	PA_Init3D(); // Uses Bg0
	PA_Reset3DSprites();
	
	// Initialise the text system on the top screen
	PA_InitText(0, 1);
	PA_InitText(1, 1);  // Initialise the text system on the top screen	
						
	PA_OutputSimpleText(1, 2, 6, " 3d Sprite Example, 1024 64x64sprites resized to 16x16 and moving around !");	
	
	// First, create the gfx with the corresponding images and sizes. Images converted as 16bit textures in PAGfx
	gfx[0] = PA_3DCreateTex((void*)mollusk_Texture,  // Texture
									64, 64,						// Width, Height
									TEX_16BITS );				// Texture Format
	gfx[1] = PA_3DCreateTex((void*)AnarX_Texture, 64, 64, TEX_16BITS);	
	gfx[2] = PA_3DCreateTex((void*)Abrexxes_Texture, 64, 64, TEX_16BITS);

	s32 i;
	u16 angle;
	for(i = 0; i < N_SPRITES; i++) {
	   sprites[i].x = 16+(PA_Rand()%224);
	   sprites[i].y = 16+(PA_Rand()%160);
		PA_3DCreateSpriteFromTex(i, 					// Sprite number
								gfx[PA_Rand()%3], // Gfx, random one :p
								64, 64, 				// Width, Height
								0, 					// Palette
								sprites[i].x, sprites[i].y); // X, Y
		PA_3DSetSpriteWidthHeight(i, 16, 16); // Resize to way smaller ^^
		angle = PA_Rand()&511; // random angle
		sprites[i].vx = PA_Cos(angle)>>7;
		sprites[i].vy = -PA_Sin(angle)>>7;
	}	



	while(1) {

		for(i = 0; i < N_SPRITES; i++){ // Move them around and change speed if touches screen
		   sprites[i].x += sprites[i].vx;
		   if(((sprites[i].x <= 8)&&(sprites[i].vx < 0))||((sprites[i].x>=255-8)&&(sprites[i].vx > 0))) sprites[i].vx = -sprites[i].vx;
		   sprites[i].y += sprites[i].vy;
		   if(((sprites[i].y <= 8)&&(sprites[i].vy < 0))||((sprites[i].y>=191-8)&&(sprites[i].vy > 0)))	sprites[i].vy = -sprites[i].vy;
			PA_3DSetSpriteXY(i, sprites[i].x, sprites[i].y); // Set new position
		} 
		

		PA_WaitForVBL();
		PA_3DProcess();  // Update 3DSprites
	}


	return 0;
} // End of main()

