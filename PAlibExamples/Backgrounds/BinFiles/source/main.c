// Load a simple background, very easy and simple...

// Includes
#include <PA9.h>       // Include for PA_Lib



// Converted using PAGfxConverter
#include "gfx/all_gfx.h" // Only include the .h
// all_gfx.c not included, we're using the .bin files in the data folder :)

// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	// Load Backgrounds with their palettes !
	PA_EasyBgLoad(0, // screen
					3, // background number (0-3)
					bg0); // Background name, used by PAGfx...
	PA_EasyBgLoad(1, 1, bg0);	


	// Infinite loop to keep the program running
	while (1)
	{
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()

