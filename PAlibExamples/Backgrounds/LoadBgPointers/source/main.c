// This example just shows how to use pointers with backgrounds...

// Includes
#include <PA9.h>       // Include for PA_Lib

// Graphics converted using PAGfxConverter
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"

PAGfx_struct *bg[4] = {&bg0, &bg1, &bg2, &bg3};   // Pointer array

// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL

	u8 i;
	
	// Load the 4 Backgrounds
	for(i = 0; i < 4; i++)	PA_EasyBgLoadPtr(0, i, bg[i]); // Takes pointeurs ! Much easier to have different levels ^^	
	
	// Normal Loading
	PA_EasyBgLoad(1, 0, bg0);	 // Normal background loading	
	PA_EasyBgLoad(1, 1, bg1);	 // Normal background loading
	PA_EasyBgLoad(1, 2, bg2);	 // Normal background loading
	PA_EasyBgLoad(1, 3, bg3);	 // Normal background loading
			
	// Infinite loop to keep the program running
	while (1)
	{
      PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
