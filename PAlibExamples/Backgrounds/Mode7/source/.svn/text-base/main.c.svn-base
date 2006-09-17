// Mode 7 example !!!

// Includes
#include <PA9.h>       // Include for PA_Lib

#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"




// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	
	PA_SetVideoMode(0, 2);  //screen, mode
	PA_SetVideoMode(1, 2);  //screen, mode
	
	PA_LoadPAGfxRotBg(0, //screen
					  3, // background number
					  Rot, // background name in PAGfx
					  1); // wraparound !
	PA_LoadPAGfxRotBg(1, 3, Rot, 1); // wraparound !
			
			
PA_InitText(1, 0);

PA_InitMode7(3);

u16 angle = 0; 
u16 height = 8192;

while (1)
{
	// Change the angle
	angle += Pad.Held.Right - Pad.Held.Left;
	angle &= 511;
	PA_Mode7Angle(angle);
	
	// Move left/right
	PA_Mode7MoveLeftRight(Pad.Held.A - Pad.Held.Y);
	
	// Move Forward/backward
	PA_Mode7MoveForwardBack(Pad.Held.Up - Pad.Held.Down);
	
	// Height
	height += (Pad.Held.X - Pad.Held.B)<<7;
	PA_Mode7Height(height);
	
	

	PA_OutputText(1, 0, 0, "Angle  : %d    ", angle);	
	PA_OutputText(1, 0, 1, "Height : %d    ", height);	
	
	PA_WaitForVBL();
}
	
	return 0;
} // End of main()
