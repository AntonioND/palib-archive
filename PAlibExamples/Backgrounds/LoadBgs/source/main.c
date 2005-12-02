// This example just shows how the DS can be intelligent... Loading 4 different backgrounds, each with it's own palette...

// Includes
#include <PA9.h>       // Include for PA_Lib



// Graphics converted using PAGfxConverter
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"

// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL

	// Load the 4 Backgrounds
	PA_LoadTiledBg(0, 0, bg0);	
	PA_LoadTiledBg(0, 1, bg1);	
	PA_LoadTiledBg(0, 2, bg2);	
	PA_LoadTiledBg(0, 3, bg3);	
	
	// Infinite loop to keep the program running
	while (1)
	{
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
