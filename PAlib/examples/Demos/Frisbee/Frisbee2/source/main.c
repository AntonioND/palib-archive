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

frisinfos frisbee[10];  // 10 Frisbees !!



int main(void)
{

// Initialise the lib...
PA_Init();
PA_InitVBL(); 

PA_InitText(1, 0);

// Load the palettes for the sprites on both screens
PA_DualLoadSpritePal(0, (void*)sprite0_Pal);

s32 i; // will be used in for loops to cycle through the frisbees...

PA_InitRand(); // Init the random stuff...

for (i = 0; i < 10; i++){
	// Sprite initial position...
	frisbee[i].x = (PA_Rand()%256)-16; // random position on the screen
	frisbee[i].y = 192+SCREENHOLE + (PA_Rand()%192)-16; // random position on the bottom screen; 
	
	// Speed of frisbee in both ways
	frisbee[i].vx = 0;
	frisbee[i].vy = 0;
	
	frisbee[i].angle = 0;	
	
	// Create the sprite on both screens...
	PA_DualCreateSprite(FRISBEE+i, (void*)frisbee_Sprite, OBJ_SIZE_32X32, 1, 0, frisbee[i].x-16, frisbee[i].y-16); 
	PA_DualSetSpriteRotEnable(FRISBEE+i, i); // Enable rotation/zoom, rotset 0
}

	while(1)
	{
		for (i = 0; i < 10; i++){
			// Move with the stylus, or move on...
			if (PA_MoveSprite(FRISBEE+i)){
				frisbee[i].x = PA_MovedSprite.X;
				frisbee[i].y = PA_MovedSprite.Y + 192 + SCREENHOLE;
				frisbee[i].vx = PA_MovedSprite.Vx;		frisbee[i].vy = PA_MovedSprite.Vy; 
			}
			else{
				// Now, the frisbee's fixed point position will be updated according to the speed...
				frisbee[i].x += frisbee[i].vx;
				frisbee[i].y += frisbee[i].vy;
			
				// If the sprite touches the left or right border, flip the horizontal speed
				if ((frisbee[i].x - 16 <= 0) && (frisbee[i].vx < 0)) frisbee[i].vx = -frisbee[i].vx; 
				else if ((frisbee[i].x + 16 >= 256)&&(frisbee[i].vx > 0)) frisbee[i].vx = - frisbee[i].vx;
		
				// Same thing, for top and bottom limits...
				if ((frisbee[i].y - 16 <= 0) && (frisbee[i].vy < 0)) frisbee[i].vy = -frisbee[i].vy;
				else if ((frisbee[i].y + 16 >= 192 + 192 + SCREENHOLE)&& (frisbee[i].vy > 0)) frisbee[i].vy = - frisbee[i].vy;		
				// The bottom limit is at the bottom of the bottom screen, so that would be 2 screen heights, plus the space in between...
				PA_DualSetSpriteXY(FRISBEE+i, frisbee[i].x-16, frisbee[i].y-16);
		
			}
			frisbee[i].angle+=4; // Make the frisbee turn...
			PA_DualSetRotsetNoZoom(i, frisbee[i].angle);
		}
		
	
	PA_WaitForVBL();  // Synch to the framerate...
	}

return 0;
}

// That's all folks, easy, right ? :p

