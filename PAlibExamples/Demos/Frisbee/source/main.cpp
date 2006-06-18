// Frisbee future game !

#include <PA9.h>

// Include the sprites :
#include "sprites/frisbee.raw.c"
#include "sprites/sprite.pal.c"


#define FRISBEE 10 // Sprite number...
#define SCREENHOLE 48 // Size of the space between the screens... This is what looked the best


typedef struct{
	s32 spritex, spritey; // This'll be the frisbee center position, in pixels
	s32 x, y; // This is for the frisbee's center position, in pixels << 8 (*256), which is fixed points...
	s32 vx, vy; // Frisbee speed, in fixed point...
	s16 angle; // Firsbee angle (0-511)
}frisinfos;

frisinfos frisbee;  // Frisbee structure variable



int main(void)
{

// Initialise the lib...
PA_Init();
PA_InitVBL(); 

s32 i; // This'll be used for the for(...)


// Load the palettes for the sprites
PA_LoadPal(PAL_SPRITE0, sprite_Palette);
PA_LoadPal(PAL_SPRITE1, sprite_Palette);


// Create the sprite on both screens...
for (i = 0; i < 2; i++) { // for both screens...
	PA_CreateSprite(i, FRISBEE, (void*)frisbee_Bitmap, OBJ_SIZE_32X32, 1, 0, 0, 192); // Y = 192, so they're hidden
	PA_SetSpriteRotEnable(i, FRISBEE, 0); // Enable rotation/zoom, rotset 0
}


// Sprite initial position...
frisbee.spritex = 64; 
frisbee.spritey = 192 + SCREENHOLE + 64; // on the bottom screen

// Since we use fixed point for the position, this will convert the sprite position to the fixed point position...
frisbee.x = frisbee.spritex << 8; // fixed point
frisbee.y = frisbee.spritey << 8; // fixed point

// Speed of 2 in both ways, and as it's fixed points, << 8
frisbee.vx = 2 << 8;
frisbee.vy = 2 << 8;

// Angle of 0 to start out
frisbee.angle = 0;

	while(1){
	
		// The firsbee turns all the time... changing the 8 changes the turning speed
		frisbee.angle -= 8;
		frisbee.angle&=511; // This limits the angle range to 0-511
		
		// Now, the frisbee's fixed point position will be updated according to the speed...
		frisbee.x += frisbee.vx;
		frisbee.y += frisbee.vy;
		
		// Then we convert the position back to pixel position, for positionning the sprite
		frisbee.spritex = frisbee.x >> 8;
		frisbee.spritey = frisbee.y >> 8;		
		
		// If the sprite touches the left border, flip the horizontal speed
		if (frisbee.spritex - 16 <= 0) frisbee.vx = - frisbee.vx; // -16 because spritex is the sprite's center...
		// If it touches the other side... +16 because spritex is the center...
		else if (frisbee.spritex + 16 >= 256) frisbee.vx = - frisbee.vx;

		// Same thing, for top and bottom limits...
		if (frisbee.spritey - 16 <= 0) frisbee.vy = - frisbee.vy;
		else if (frisbee.spritey + 16 >= 192 + 192 + SCREENHOLE) frisbee.vy = - frisbee.vy;		
		// The bottom limit is at the bottom of the bottom screen, so that would be 2 screen heights, plus the space in between...


		// Now comes the important part : showing the sprite from one screen to another !
		if (frisbee.spritey < 192 + SCREENHOLE) { // visible on top screen
			PA_SetRotset(1, 0, frisbee.angle, 256, 256); // Rotation (and zoom later on) on top screen
			PA_SetSpriteXY(1, FRISBEE, frisbee.spritex-16, frisbee.spritey-16); // Position on top screen (-16...)
		}
		if (frisbee.spritey > 192) { // visible on bottom screen, same thing as top screen...
			PA_SetRotset(0, 0, frisbee.angle, 256, 256);
			PA_SetSpriteXY(0, FRISBEE, frisbee.spritex-16, frisbee.spritey-16-(192+SCREENHOLE));
		}		

		
	
	PA_WaitForVBL();  // Synch to the framerate...
	
	
	}

return 0;
}

// That's all folks, easy, right ? :p

