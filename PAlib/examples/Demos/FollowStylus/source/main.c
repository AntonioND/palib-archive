
// Includes
#include <PA9.h>

// PAGfxConverter Include
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"

int main(void){

	PA_Init();
	PA_InitVBL();
	
	PA_InitText(1,0); // On the top screen

	PA_LoadSpritePal(0, 0, (void*)sprite0_Pal);

	PA_CreateSprite(0, 0,(void*)vaisseau_Sprite, OBJ_SIZE_32X32,1, 0, 128-16, 96-16); // Create the ship in the center...
	PA_SetSpriteRotEnable(0,0,0);// Enable rotations and use Rotset 0... 
	
	s32 x = (128) << 8; // ship x position in 8bit fixed point
	s32 y = (96) << 8; // Y
	u16 angle = 0; // direction in which to move !
	
	

	while(1)
	{
		angle = PA_GetAngle(x>>8, y>>8, Stylus.X, Stylus.Y);
		PA_SetRotsetNoZoom(0, 0, angle); // Turn the ship in the correct direction
		
		if (Stylus.Held){ // Move forward
			x += PA_Cos(angle);
			y -= PA_Sin(angle);
		}

		PA_OutputText(1, 5, 10, "Angle : %d  ", angle);
	
		PA_SetSpriteXY(0, 0, (x>>8)-16, (y>>8)-16); // Sprite position converted to normal...
		
		PA_WaitForVBL();

	}
	return 0;
}
