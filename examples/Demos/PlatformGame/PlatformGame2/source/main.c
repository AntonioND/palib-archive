// Includes, only one sprite
#include <PA9.h>

#include "all_gfx.h"

typedef struct{
	s32 x, y;
	s32 vy; // used for jumping...
	s32 flip;
} mariotype;

mariotype mario;

#define COL_MAP ((u16*)mario_world.BgMap)

#define GRAVITY 48
#define MARIO_SPEED 512


void MoveMario(void);
void CheckCollisions(void);
u8 GetTile(s16 x, s16 y);
u8 LeftCollision(void);
u8 RightCollision(void);
u8 DownCollision(void);
u8 TouchingGround(void);


// Main function
int main(void)	{
	// PAlib init
	PA_Init();
	
	PA_LoadDefaultText(1, 0);

	PA_LoadSpritePal(0, 0, (void*)sprite0_Pal);	// Palette....	

	PA_LoadBackground(0, 1, &mario_world); // platfroms...
	PA_LoadBackground(0, 3, &back); // back

	
	mario.x = 0<<8; mario.y = (128-32)<<8; // bottom of the screen... fixed point
	mario.vy = 0; // not jumping
	mario.flip = 0;
	PA_CreateSprite(0, 0,(void*)mario_Sprite, OBJ_SIZE_32X32,1, 0, mario.x>>8, mario.y>>8); // Sprite
	
	while(1)
	{
		MoveMario();
	
		PA_SetSpriteXY(0, 0, mario.x>>8, mario.y>>8);
		
		PA_OutputText(1, 2, 9, "X : %d   ", mario.x >> 8);
		PA_OutputText(1, 2, 10, "Y : %d   ", mario.y >> 8);	
	
		PA_WaitForVBL();
	}
	
	return 0;
}



void MoveMario(void){
	if(Pad.Newpress.Right) {
		PA_StartSpriteAnim(0, 0, 0, 1, 6);	
		PA_SetSpriteHflip(0, 0, 0);
		mario.flip = 0;
	}
	else if(Pad.Newpress.Left) {
		PA_StartSpriteAnim(0, 0, 0, 1, 6);	
		PA_SetSpriteHflip(0, 0, 1);
		mario.flip = 1;
	}
	
	if ((Pad.Newpress.A) && (TouchingGround())){  // If pressed A and not in the air
		mario.vy = -1200; // Start jumping
	}

	// Moving Code
	mario.x += (Pad.Held.Right - Pad.Held.Left)*MARIO_SPEED;	 // in fixed point...
	
	// Add gravity
	mario.vy += GRAVITY;
	mario.y += mario.vy;
	
	CheckCollisions();
	
	if (!TouchingGround()) PA_SetSpriteAnim(0, 0, 2); // Not on the ground
	else if(!((Pad.Held.Left)||(Pad.Held.Right))) PA_SetSpriteAnim(0, 0, 0);// Image if not in the air and not walking
}





u8 GetTile(s16 x, s16 y){
	if (x < 0) return 1; // Say it was a collision...
	return COL_MAP[((y>>3)*32) + (x>>3)];
}

u8 LeftCollision(void){
	return GetTile((mario.x>>8)+2, (mario.y>>8)+8+(mario.flip*13));
}

u8 RightCollision(void){
	return (GetTile((mario.x>>8)+29, (mario.y>>8)+8 + ((!mario.flip)*13)));
}

u8 DownCollision(void){
	return (mario.vy >= 0 && GetTile((mario.x>>8)+10 + (mario.flip*11), (mario.y>>8)+31));
}

u8 TouchingGround(void){
	return GetTile((mario.x>>8)+10 + (mario.flip*11), (mario.y>>8)+32);
}

void CheckCollisions(void){

	while(LeftCollision()){ // Collision on the left of the sprite...
		mario.x+= 256; // Move by 1 pixel...
	}
	while(RightCollision()){ // Collision on the right of the sprite...
		mario.x-= 256; // Move by 1 pixel...
	}
	
	while(DownCollision()){ // Collision on the bottom of the sprite...
		mario.y -= 128; // Move by 1/2 pixel...
		mario.vy = 0; // TOuched the floor...
	}
	if(TouchingGround()) mario.vy = 0;

}