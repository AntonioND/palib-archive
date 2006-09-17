// Plays a sound on lid close/open

// Includes
#include <PA9.h>       // Include for PA_Lib
#include "Babye.h"  // Include the sound (found in the data folder in .raw format)
#include "HereWeGo.h"  // Include the sound (found in the data folder in .raw format)


// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	PA_InitText(0, 0);
	
	
	PA_InitSound();  // Init the sound system
	
	PA_OutputSimpleText(0, 1, 10, "Close the lid and see what     happens...");

	// Infinite loop to keep the program running
	while (1)
	{	
		PA_CloseLidSound2(0, Babye, HereWeGo); // Channel, Sound to play when lid closes and opens

		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
