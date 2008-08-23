// This example shows how to use 8 bit backgrounds, 
// on which you can draw with the stylus or show text in different sizes, colors, or rotations

// Includes
#include <PA9.h>       // Include for PA_Lib




// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	//PA_LoadSplash();  // PA_Lib splash screen
	
	// Let's put some colors... in the background palettes
	PA_SetBgPalCol(0, 1, PA_RGB(31, 31, 31));
	PA_SetBgPalCol(0, 2, PA_RGB(31, 0, 0));	
	PA_SetBgPalCol(0, 3, PA_RGB(0, 0, 31));
	PA_SetBgPalCol(0, 4, PA_RGB(0, 31, 0));

	// Let's not load any backgrounds :p
	
	// This will initialise an  bit background on each screen. This must be loaded before any other background.
	// If you need to load this after a backgrounds, you'll have to use PA_ResetBgSys, PA_Init8bit, then reload
	// your backgrounds...
	PA_Init8bitBg(0, 3);

	PA_SmartText(0, 0, 80, 255, 100, "Hello World", 1, 4, 1, 100);


	// Let's have some rotated text :p
	// X and Y positions are taken as if the DS was turned
	PA_SmartText(0, 1, 1, 190, 20, "Hello World", 3, 3, 3, 100);

	// And a centered text, to check if it works
	PA_CenterSmartText(0, 0, 100, 255, 120, "Hello World", 2, 4, 1); // Same as Smart text, but no letter limit
	
	
	
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
