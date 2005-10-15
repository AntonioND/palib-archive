// Custom splash screens !

// Includes
#include <PA9.h>       // Include for PA_Lib




// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL


	// Load Palettes
	PA_LoadPal(PAL_BG0, splashPal); // Backgrounds
	PA_LoadPal(PAL_BG1, splashPal); // Backgrounds
	
	// I'll use the splash function to load the PA normal splash...
	PA_Splash((void*)splashData,(void*)bitmap, // bottom screen tiles and map to load
			(void*)splash2Data, (void*)bitmap, // top screen tiles and map
			1, 0, // fade in (1) from white, fade out (0) to black
			300); // Stay 5 seconds if nothing is pressed (Start/Stylus)
	

	// Infinite loop to keep the program running
	while (1)
	{

		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
