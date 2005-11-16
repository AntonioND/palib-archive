// Mode 7 example !!!

// Includes
#include <PA9.h>       // Include for PA_Lib



// Graphics Includes
// *** Backgrounds ***
// Be carefull, it's not exactly the same gfx2gba command... 
// gfx2gba -m -mm -fsrc -rs -t8 -T8 -pbg.pal *.bmp
#include "gfx/Rot.map.c"
#include "gfx/Rot.raw.c"
#include "gfx/bg.pal.c"




// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	//PA_LoadSplash();  // PA_Lib splash screen
	
	// Load Palettes
	PA_LoadPal(PAL_BG0, bg_Palette); // Backgrounds
	PA_LoadPal(PAL_BG1, bg_Palette); // Backgrounds
	
	PA_SetVideoMode(0, 2);  //screen, mode
	PA_SetVideoMode(1, 2);  //screen, mode
	
PA_LoadRotBg(0, // screen
			3, // background number
			Rot_Tiles, // Tiles
			Rot_Map,  // Map
			BG_ROT_256X256, // Size
			1);  // Wraparound
PA_LoadRotBg(1, 3, Rot_Tiles, Rot_Map, BG_ROT_256X256, 1);  // Wraparound
			
			
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
