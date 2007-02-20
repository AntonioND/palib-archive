// Includes, only one sprite
#include <PA9.h>


// PAGfxConverter Include
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"


typedef struct{
	s32 x, y;
	s32 vy; // used for jumping...
} mariotype;

mariotype mario;

#define GRAVITY 48


void MoveMario(void);


// Main function
int main(void)	{
	// PAlib init
	PA_Init();
	PA_InitVBL();
	
	PA_InitText(1, 0);

	PA_LoadSpritePal(0, 0, (void*)sprite0_Pal);	// Palette....	

	mario.x = 0<<8; mario.y = (192-32)<<8; // bottom of the screen... fixed point
	mario.vy = 0; // not jumping
	PA_CreateSprite(0, 0,(void*)mario_Sprite, OBJ_SIZE_32X32,1, 0, mario.x>>8, mario.y>>8); // Sprite
	
	while(1)
	{
		MoveMario();
	
		PA_SetSpriteXY(0, 0, mario.x>>8, mario.y>>8);
	
		PA_WaitForVBL();
	}
	
	return 0;
}



void MoveMario(void){
	if(Pad.Newpress.Right) {
		PA_StartSpriteAnim(0, 0, 0, 1, 6);	
		PA_SetSpriteHflip(0, 0, 0);
	}
	else if(Pad.Newpress.Left) {
		PA_StartSpriteAnim(0, 0, 0, 1, 6);	
		PA_SetSpriteHflip(0, 0, 1);
	}
	
	if ((Pad.Newpress.A) && (mario.vy == 0)){  // If pressed A and not in the air
		mario.vy = -1000; // Start jumping
	}

	// Moving Code
	mario.x += (Pad.Held.Right - Pad.Held.Left)<<8;	 // in fixed point...
	
	// Add gravity
	mario.vy += GRAVITY;
	mario.y += mario.vy;
	if (mario.y >= (192-32)<<8) {
		mario.y = (192-32)<<8;
		mario.vy = 0;
	}
	
	if (mario.vy != 0) PA_SetSpriteAnim(0, 0, 2); // If going up or down, means the sprite is jumping !
	else if(!((Pad.Held.Left)||(Pad.Held.Right))) PA_SetSpriteAnim(0, 0, 0);// Image if not in the air and not walking
}
