
// Includes
#include <PA9.h>

// PAGfxConverter Include
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"

typedef struct{
	s16 x, y; // position
	s16 vx, vy; // speed
}puckinfos;

puckinfos puck;

#define SCREENHOLE 48

int main(void){

	PA_Init();
	PA_InitVBL();
	
	PA_InitText(1,0); // On the top screen

	PA_DualLoadSpritePal(0, (void*)sprite0_Pal);
	
	// This'll be the movable sprite...
	PA_CreateSprite(0, 0,(void*)circle_Sprite, OBJ_SIZE_32X32,1, 0, 16, 16); 	
	s32 x = 16; s32 y = 16; // Sprite's center position
	
	// This will be the hit circle
	PA_DualCreateSprite(1,(void*)circle_Sprite, OBJ_SIZE_32X32,1, 0, 128-16, 96-16); 
	puck.x = 128; puck.y = 96+192+SCREENHOLE; // central position on bottom screen
	puck.vx = 0; puck.vy = 0; // No speed
	
	
	while(1)
	{
		if (PA_MoveSprite(0)){
			x = PA_MovedSprite.X;
			y = PA_MovedSprite.Y;
		}
		
		// Collision ?
		if (PA_Distance(x, y, puck.x, puck.y-192-SCREENHOLE) < 32*32) {
			// Collision, so we'l change the pucks speed to move it out of our 'raquette'
			u16 angle = PA_GetAngle(x, y, puck.x, puck.y-192-SCREENHOLE); // New direction angle
			u16 speed = (32*32-PA_Distance(x, y, puck.x, puck.y-192-SCREENHOLE))/32; // The closer they are, the harder the hit was...
			puck.vx = (PA_Cos(angle)*speed)>>8;
			puck.vy = -(PA_Sin(angle)*speed)>>8;
		}
		
		puck.x += puck.vx;
		puck.y += puck.vy;
	
		// If the sprite touches the left or right border, flip the horizontal speed
		if ((puck.x -16 <= 0) && (puck.vx < 0)) puck.vx = -puck.vx; 
		else if ((puck.x + 16 >= 256)&&(puck.vx > 0)) puck.vx = - puck.vx;

		// Same thing, for top and bottom limits...
		if ((puck.y -16 <= 0) && (puck.vy < 0)) puck.vy = -puck.vy;
		else if ((puck.y + 16 >= 192 + 192 + SCREENHOLE)&& (puck.vy > 0)) puck.vy = - puck.vy;		
		// The bottom limit is at the bottom of the bottom screen, so that would be 2 screen heights, plus the space in between...
		PA_DualSetSpriteXY(1, puck.x-16, puck.y-16);
		
		

		PA_WaitForVBL();
	}
	return 0;
}
