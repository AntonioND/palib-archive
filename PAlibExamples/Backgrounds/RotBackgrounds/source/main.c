// And here are the rotation backgrounds !!!!
// They do not work on Dualis, though :(

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
	
PA_LoadRotBg(0, // screen
			3, // background number
			Rot_Tiles, // Tiles
			Rot_Map,  // Map
			BG_ROT_256X256, // Size
			1);  // Wraparound
		
PA_InitText(1, 0);

		
	// Infinite loop to keep the program running
s32 scrollx = 0;
s32 scrolly = 0;
s32 rotcenterx = 0;
s32 rotcentery = 0;
s16 angle = 0;
s32 zoom = 256;

while (1)
{
	zoom += Pad.Held.Start - Pad.Held.Select;
	scrolly += Pad.Held.Right - Pad.Held.Left;
	scrolly += Pad.Held.Down - Pad.Held.Up;
	rotcenterx += Pad.Held.B - Pad.Held.X;
	rotcentery += Pad.Held.A - Pad.Held.Y;	
	angle += Pad.Held.R - Pad.Held.L;
	
	PA_OutputText(1, 0, 0, "Zoom : %d  ", zoom);
	
	
	PA_SetBgRot(0, 3, scrollx, scrolly, rotcenterx, rotcentery, angle, zoom);
	
	PA_WaitForVBL();
}
	
	return 0;
} // End of main()
