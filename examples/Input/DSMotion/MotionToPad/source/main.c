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
	
	PA_InitText(0, 0);  // Initialise the text system on the top screen	
	PA_InitText(1, 0);  // Initialise the text system on the top screen

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
	
	PA_ClearTextBg(1); // erase the text...
	
	PA_MotionToPad(1); // Overrides the pad keys and puts the DS Motion instead

	PA_LoadSpritePal(0, 0, (void*)sprite0_Pal);	// Palette....	

	s32 x = 120; s32 y = 64;

	PA_CreateSprite(0, 0,(void*)som_Sprite, OBJ_SIZE_16X32,1, 0, x, y); // Sprite
	
	while(1)
	{
		// Animation code...
		if(Pad.Newpress.Up) PA_StartSpriteAnim(0, 0, 0, 3, 6);
		if(Pad.Newpress.Down) PA_StartSpriteAnim(0, 0, 8, 11, 6);		
		
		if(Pad.Newpress.Right) {
			PA_StartSpriteAnim(0, 0, 4, 7, 6);	
			PA_SetSpriteHflip(0, 0, 0);
		}
		if(Pad.Newpress.Left) {
			PA_StartSpriteAnim(0, 0, 4, 7, 6);	
			PA_SetSpriteHflip(0, 0, 1);
		}

		
		if(!((Pad.Held.Left)||(Pad.Held.Up)||(Pad.Held.Down)||(Pad.Held.Right))) PA_SpriteAnimPause(0, 0, 1);
	
	
		// Moving Code
		y += Pad.Held.Down - Pad.Held.Up;
		x += Pad.Held.Right - Pad.Held.Left;		
		PA_SetSpriteXY(0, 0, x, y);
		
		PA_OutputText(1, 3, 7, "Up     : %d-%d-%d", Pad.Newpress.Up, Pad.Held.Up, Pad.Released.Up);
		PA_OutputText(1, 3, 8, "Down   : %d-%d-%d", Pad.Newpress.Down, Pad.Held.Down, Pad.Released.Down);
		PA_OutputText(1, 3, 9, "Left   : %d-%d-%d", Pad.Newpress.Left, Pad.Held.Left, Pad.Released.Left);		
		PA_OutputText(1, 3, 10, "Right  : %d-%d-%d", Pad.Newpress.Right, Pad.Held.Right, Pad.Released.Right);	
	
		PA_WaitForVBL();
	}
	
	return 0;
}
