// And here are the rotation backgrounds !!!!
// They do not work on Dualis, though :(

// Includes
#include <PA9.h>       // Include for PA_Lib


#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"



// Function: main()
int main(int argc, char ** argv){
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	PA_SetVideoMode(0, 2);  //screen, mode

	PA_LoadPAGfxRotBg(0, //screen
					  3, // background number
					  Rot, // background name in PAGfx
					  1); // wraparound !
	
	PA_InitText(1, 0);
	
		// Infinite loop to keep the program running
	s32 scrollx = 0;
	s32 scrolly = 0;
	s32 rotcenterx = 0;
	s32 rotcentery = 0;
	s16 angle = 0;
	s32 zoom = 256;
	
	PA_OutputSimpleText(1, 2, 2, "Zoom       : Start/Select");
	PA_OutputSimpleText(1, 2, 3, "ScrollX    : Left/Right");
	PA_OutputSimpleText(1, 2, 4, "Scrolly    : Up/Down");
	PA_OutputSimpleText(1, 2, 5, "RotCenterX : A/Y");
	PA_OutputSimpleText(1, 2, 6, "RotCenterY : B/X");
	PA_OutputSimpleText(1, 2, 7, "Angle      : R/L");


	while (1)
	{
		zoom += Pad.Held.Start - Pad.Held.Select;
		scrollx += Pad.Held.Right - Pad.Held.Left;
		scrolly += Pad.Held.Down - Pad.Held.Up;
		rotcenterx += Pad.Held.A - Pad.Held.Y;
		rotcentery += Pad.Held.B - Pad.Held.X;	
		angle += Pad.Held.R - Pad.Held.L;
		
		PA_SetBgRot(0, 3, scrollx, scrolly, rotcenterx, rotcentery, angle, zoom);
		
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
