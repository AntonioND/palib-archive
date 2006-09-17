// Frisbee future game !

#include <PA9.h>

// Include the sprites :
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"


#define FRISBEE 10 // Sprite number...
#define SCREENHOLE 48 // Size of the space between the screens... This is what looked the best


typedef struct{
	s16 x, y; // This is for the frisbee's position
	s16 vx, vy; // Frisbee speed
	s16 angle; // To make the frisbee turn on itself
}frisinfos;

frisinfos frisbee;  // Frisbee structure variable



int main(void)
{

// Initialise the lib...
PA_Init();
PA_InitVBL(); 

PA_InitText(1, 0);

// Load the palettes for the sprites on both screens
PA_DualLoadSpritePal(0, (void*)sprite0_Pal);

// Create the sprite on both screens...
PA_DualCreateSprite(FRISBEE, (void*)frisbee_Sprite, OBJ_SIZE_32X32, 1, 0, 96, 300); // Bottom screen
PA_DualSetSpriteRotEnable(FRISBEE, 0); // Enable rotation/zoom, rotset 0

// Sprite initial position...
frisbee.x = 96+16; 
frisbee.y = 300+16; // on the bottom screen

// Speed of frisbee in both ways
frisbee.vx = 0;
frisbee.vy = 0;

	while(1)
	{
		// Move with the stylus, or move on...
		if (PA_MoveSprite(FRISBEE)){
			frisbee.x = PA_MovedSprite.X;
			frisbee.y = PA_MovedSprite.Y + 192 + SCREENHOLE;
			frisbee.vx = PA_MovedSprite.Vx;		frisbee.vy = PA_MovedSprite.Vy; 
		}
		else{
			// Now, the frisbee's fixed point position will be updated according to the speed...
			frisbee.x += frisbee.vx;
			frisbee.y += frisbee.vy;
		
			// If the sprite touches the left or right border, flip the horizontal speed
			if ((frisbee.x -16 <= 0) && (frisbee.vx < 0)) frisbee.vx = -frisbee.vx; 
			else if ((frisbee.x + 16 >= 256)&&(frisbee.vx > 0)) frisbee.vx = - frisbee.vx;
	
			// Same thing, for top and bottom limits...
			if ((frisbee.y -16 <= 0) && (frisbee.vy < 0)) frisbee.vy = -frisbee.vy;
			else if ((frisbee.y + 16 >= 192 + 192 + SCREENHOLE)&& (frisbee.vy > 0)) frisbee.vy = - frisbee.vy;		
			// The bottom limit is at the bottom of the bottom screen, so that would be 2 screen heights, plus the space in between...
			PA_DualSetSpriteXY(FRISBEE, frisbee.x-16, frisbee.y-16);
	
		}
		
		PA_OutputText(1, 2, 10, "SpeedX : %d    ", frisbee.vx);
		PA_OutputText(1, 2, 11, "SpeedY : %d    ", frisbee.vy);		
		frisbee.angle+=4; // Make the frisbee turn...
		PA_DualSetRotsetNoZoom(0, frisbee.angle);
		
	
	PA_WaitForVBL();  // Synch to the framerate...
	}

return 0;
}

// That's all folks, easy, right ? :p

