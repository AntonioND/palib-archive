// Load an 16 bit jpeg on the screen !
// The jpeg must be put in the data folder...



// Includes
#include <PA9.h>       // Include for PA_Lib

#include "Photo.h" // Include the jpeg...

// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	

	// This will initialise a 16 bit background on each screen. This must be loaded before any other background.
	// If you need to load this after a backgrounds, you'll have to use PA_ResetBgSys, PA_Init16bit, then reload your backgrounds...
	PA_Init16bitBg(0, 3);
	PA_Init16bitBg(1, 3);

	// Load the jpeg on the given screen... pretty simple !
	PA_LoadJpeg(0, (void*)Photo);
	PA_LoadJpeg(1, (void*)Photo);		
	
	// Infinite loop to keep the program running
	while (1)
	{
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
