
// Includes
#include <PA9.h>

// PAGfxConverter Include
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"

#define MAXBLUE 4 // number of blue ones...

typedef struct{
	s16 x, y; // position in pixels
	s32 fx, fy; // Position in fixed point
	s32 vx, vy; // speed...
	s16 w, h; // HALF width and HALF height, because that's what we'll use...
} rectangle;

rectangle blue[MAXBLUE]; // MAXBLUE possible rectangles, we'll probably use less

rectangle red; // Our rectangle...


// Blue rectangle inits
void Blue16x32(u8 number);
void Blue32x16(u8 number);
void Blue32x32(u8 number);

void MoveBlue(u8 number); // Blue rect move
void CheckCollision(u8 number); // Checks if there is a collision...


// Fixed-Normal conversions
extern inline void ToFixedPoint(u8 number){
	blue[number].fx = blue[number].x <<8;
	blue[number].fy = blue[number].y <<8;	
}

extern inline void ToNormal(u8 number){
	blue[number].x = blue[number].fx >>8;
	blue[number].y = blue[number].fy >>8;	
}



int main(void){

	PA_Init();
	PA_InitVBL();
	
	PA_InitText(1,0); // On the top screen

	PA_LoadSpritePal(0, 0, (void*)sprite0_Pal);
	
	red.x = 128; red.y = 96;
	red.w = 16;  red.h = 16; // Half width and height...
	PA_CreateSprite(0, 0,(void*)red_Sprite, OBJ_SIZE_32X32,1, 0, red.x - red.w, red.y - red.h); 		
	
	PA_InitRand(); // We'll put some random stuff in there...

	s32 i;
	
	for(i = 0; i < MAXBLUE; i++){
		u8 random = PA_Rand()%3;
		if (random == 0) Blue16x32(i);
		else if (random == 1) Blue32x32(i);
		else if (random == 2) Blue32x16(i);
		
		// Position and random angle
		blue[i].x = (i&1)*256; blue[i].y = (i>>1)*192;
		u16 angle = PA_Rand()&511;
		blue[i].vx = PA_Cos(angle);
		blue[i].vy = -PA_Sin(angle);
		
		ToFixedPoint(i); // Add the fixed point values based on the position...
	}
	
	
	while(1)
	{
		if (PA_MoveSprite(0)){
			red.x = PA_MovedSprite.X;
			red.y = PA_MovedSprite.Y;
		}
		
		for (i = 0; i < MAXBLUE; i++) {
			MoveBlue(i); // Move all the blue rectangles...
			CheckCollision(i); // Check collision
		}

		PA_WaitForVBL();
	}
	return 0;
}


void Blue16x32(u8 number){
	blue[number].w = 8;  blue[number].h = 16; 
	PA_CreateSprite(0, number+1, (void*)blue_Sprite, OBJ_SIZE_16X32,1, 0, 0, 0); 	
}

void Blue32x16(u8 number){
	blue[number].w = 16;  blue[number].h = 8; 
	PA_CreateSprite(0, number+1, (void*)blue_Sprite, OBJ_SIZE_32X16,1, 0, 0, 0); 	
}

void Blue32x32(u8 number){
	blue[number].w = 16;  blue[number].h = 16; 	
	PA_CreateSprite(0, number+1, (void*)blue_Sprite, OBJ_SIZE_32X32,1, 0, 0, 0); 	
}




void MoveBlue(u8 number){

	blue[number].fx += blue[number].vx;
	blue[number].fy += blue[number].vy;
	
	ToNormal(number); // Convert to normal again, to check easily if out...

	if ((blue[number].x - blue[number].w <= 0) && (blue[number].vx < 0)) blue[number].vx = -blue[number].vx; 
	else if ((blue[number].x + blue[number].w >= 256)&&(blue[number].vx > 0)) blue[number].vx = - blue[number].vx;

	// Same thing, for top and bottom limits...
	if ((blue[number].y - blue[number].h <= 0) && (blue[number].vy < 0)) blue[number].vy = -blue[number].vy;
	else if ((blue[number].y + blue[number].h >= 192)&& (blue[number].vy > 0)) blue[number].vy = - blue[number].vy;		

	PA_SetSpriteXY(0, number+1, blue[number].x-blue[number].w, blue[number].y-blue[number].h);
}


void CheckCollision(u8 number){
		if ((red.x >= blue[number].x - (blue[number].w + red.w)) && (red.x <= blue[number].x + (blue[number].w + red.w)) && (red.y >= blue[number].y - (blue[number].h + red.h)) && (red.y <= blue[number].y + (blue[number].h + red.h))) PA_OutputText(1, 2, 10, "Collision !!");
	
}

