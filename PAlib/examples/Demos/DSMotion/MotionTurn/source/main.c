/* This will show the new sprite animation functions !! Yay !
Thanks to http://www.reinerstileset.4players.de:1059/englisch.htm and Arog for the sprite ;)
*/


// Includes, only one sprite
#include <PA9.h>


// PAGfxConverter Include
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"


s16 framecount; //Frame count...


// Main function
int main(void)	{
	// PAlib init
	PA_Init();
	PA_InitVBL();
	
	PA_InitText(1, 0);
	
	// Load the sprite palette, 
	PA_LoadSpritePal(0, // Screen
					0, // Palette number
					(void*)mario_Pal);	// Palette name
	
	// Here, we'll load a few similar sprites sprite to animate... at different speed
	PA_CreateSprite(0, 0,(void*)mario_Sprite, OBJ_SIZE_32X32,1, 0, 128-16, 96-16);
	PA_SetSpritePrio(0, 0, 1);

	
	
	PA_OutputSimpleText(1, 0, 5, "Please insert your DS Motion and press any key"); // Ask to insert the DS Motion...

	PA_WaitFor(Pad.Newpress.Anykey);  // Wait for any key to be pressed before continuing...
	PA_ClearTextBg(1);		// Erase text screen...

	
	 // Turn on the DS Motion...
	 PA_MotionInit();
	 
	while ( !PA_CheckDSMotion()) { // Checks for the DS Motion, continues looking for it until it actually finds it...
		PA_WaitForVBL();
		
		// If no DSMotion found...
		PA_OutputText(1, 0, 5, "      DS Motion not found ! \nPlease insert your DS Motion and press any key"); // Ask to insert the DS Motion... again...
		
		PA_WaitFor(Pad.Newpress.Anykey);  // Wait for any key to be pressed before continuing...
		PA_MotionInit();
	}	
	

	PA_CreateSprite(0, 1,(void*)chomp_Sprite, OBJ_SIZE_64X64,1, 0, 256, 96-32);
	PA_SetSpriteRotEnable(0, 1, 0);

	while(1)
	{
	   
	   u16 angle = Motion.Angle;
	   if(Motion.Force > 10) { // Requires a minimum force to do...
		   PA_SetSpriteAnim(0, 0, ((angle-128+32)>>6)&7); // Set frame according to DS Motion Angle ! Minimum force needed : 15
	   	PA_SetSpriteXY(0, 1, 128-32+(PA_Cos(Motion.Angle)>>3), 96-32-(PA_Sin(Motion.Angle)>>3));
	   	PA_SetRotsetNoZoom(0, 0, (Motion.Angle-128)&511);
	   	if(Motion.Angle > 255) PA_SetSpritePrio(0, 1, 0);
	   	if(Motion.Angle < 255) PA_SetSpritePrio(0, 1, 2);	   	
	 	}  	
		PA_WaitForVBL();
	}
	
	return 0;
}
