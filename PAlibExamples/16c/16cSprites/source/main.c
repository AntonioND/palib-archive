// Includes
#include <PA9.h>       // Include for PA_Lib

#include "gfx/all_gfx.h"
#include "gfx/all_gfx.c"

#define N_SHOTS 400 // number of shots


typedef struct{
	u8 screen;
	s16 screenx, screeny;
	s32 x, y, vx, vy;
} shottype;

shottype shots[N_SHOTS]; // Have 200 shots...

// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	

	// Init a 16c background for both screens
	PA_InitComplete16c(1,  // background number to use
							(void*)shot_Pal); // palette to use...
	
	s32 i;
	
	// Fill shots with random postions and directions...
	for (i = 0; i < N_SHOTS; i++){
		shots[i].x = 8+PA_Rand()%240; shots[i].x = shots[i].x << 8; // 8 bit fixed point
		shots[i].y = 8+PA_Rand()%366; shots[i].y = shots[i].y << 8;
		shots[i].screen = 0; shots[i].screenx = 256; shots[i].screeny = 0; // hide sprite at first
		s16 angle = PA_Rand()%512;
		shots[i].vx = PA_Cos(angle);
		shots[i].vy = -PA_Sin(angle);
	}
	
	
	// Infinite loop to keep the program running
	while (1)
	{
		// Move shots
		for (i = 0; i < N_SHOTS; i++) {
			// Add speed...
			shots[i].x += shots[i].vx;
			shots[i].y += shots[i].vy;
			
			// Screen positions
			shots[i].screenx = (shots[i].x >> 8); // screen position...
			shots[i].screeny = (shots[i].y >> 8); // same here...
			
			// Adjust speed to avoid moving out of screen...
			if((shots[i].screenx < 4)||(shots[i].screenx > 256-4)) shots[i].vx = -shots[i].vx;		
			if((shots[i].screeny < 4)||(shots[i].screeny > 387)) shots[i].vy = -shots[i].vy;
		
			// See on which screen we'll display it...
			if(shots[i].screeny < 199) shots[i].screen = 1; // top screen
			else {
				shots[i].screen = 0; // bottom screen
				shots[i].screeny = shots[i].screeny-200; // 8pixel space between screens, just for the sake of it !
			}		
		}
		
		PA_WaitForVBL();
		PA_Dual16cErase();	// Erase both screens right after VBL...
		
		// Display shots... must be done RIGHT AFTER the erase...
		for (i = 0; i < N_SHOTS; i++) PA_16c8X8(shots[i].screen, shots[i].screenx-4, shots[i].screeny-4, (void*)shot_Sprite);
	}
	
	return 0;
} // End of main()
