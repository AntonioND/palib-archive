// Mode 7 example.

// Includes
#include <PA9.h>

#include "all_gfx.h"

int main(){
	PA_Init();
	
	PA_SetVideoMode(0, 2);  //screen, mode
	PA_SetVideoMode(1, 2);  //screen, mode
	
	// Yup, we use the standard bg load function!
	PA_LoadBackground(0, //screen
	                  3, // background number
	                  &Rot); // background name in PAGfx
	PA_LoadBackground(1, 3, &Rot);

	// Wraparound (!)
	PA_SetBgWrap(0, 3, 1);
	PA_SetBgWrap(1, 3, 1);
			
	PA_LoadDefaultText(1, 0);

	PA_InitMode7(3);

	u16 angle = 0; 
	u16 height = 8192;

	while(true){
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
}