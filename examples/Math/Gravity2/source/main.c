#include <PA9.h>

// PAGfxConverter Include
#include "all_gfx.h"

#define FLOOR (160<<8) // Floor y level



int main(void){

	PA_Init(); //PAlib inits
	
	PA_LoadDefaultText(1, 0);
	
	PA_OutputText(1, 2, 4, "Press A to take off !");
	PA_OutputText(1, 2, 5, "Gravity change : Left/Right");	
	PA_OutputText(1, 2, 7, "Takeoff Speed change : Up/Down");
	
	PA_LoadSpritePal(0, // Screen
					0, // Palette number
					(void*)sprite0_Pal);	// Palette name
					
	PA_CreateSprite(0, // Screen
					0, // Sprite number
					(void*)vaisseau_Sprite, // Sprite name
					OBJ_SIZE_32X32, // Sprite size
					1, // 256 color mode
					0, // Sprite palette number
					50, 50); // X and Y position on the screen
					

	s32 gravity = 32; // change the gravity and check the result :)
	s32 velocity_y = 0;
	s32 spritey = 190<<8; // at the bottom
	s32 jumpheight = 1024; // Takeoff speed...
		
	while(1) // Infinite loops
	{
		
		jumpheight += (Pad.Held.Up - Pad.Held.Down); // Change takeoff speed...
		gravity += (Pad.Held.Right - Pad.Held.Left); // Change gravity speed...

		PA_OutputText(1, 4, 8, "Jump height: %d   ", jumpheight);
		PA_OutputText(1, 4, 6, "Gravity:     %d   ", gravity);	
	
		if((spritey <= FLOOR) && Pad.Newpress.A)   { // YOu can jump if not in the air...
			velocity_y = -jumpheight;  // Change the base speed to see the result...
		}
		
		// Moves all the time...
		velocity_y += gravity; // Gravity...
		spritey += velocity_y; // Speed...
			
		
		if(spritey >= FLOOR) // Gets to the floor !
		{
			velocity_y = 0;
			spritey = FLOOR;
		}	
		
		
		PA_OutputText(1, 0, 0, "Y : %d   \nVY : %d    ", spritey>>8, velocity_y);

		if (spritey>>8 > -32) PA_SetSpriteY(0, 0, spritey>>8); // show if on screen
		else PA_SetSpriteY(0, 0, 192);
		
		PA_WaitForVBL();
	}
	
return 0;
}