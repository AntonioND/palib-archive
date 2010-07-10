// Plays a sound on lid close/open

// Includes
#include <PA9.h>       // Include for PA_Lib
#include "Babye.h"  // Include the sound (found in the data folder in .raw format)
#include "HereWeGo.h"  // Include the sound (found in the data folder in .raw format)


// Function: main()
int main()
{
	PA_Init();    // Initializes PA_Lib
	PA_LoadDefaultText(0, 0);
	PA_SetAutoCheckLid(0);
	
	
	// Init the sound system
	AS_Init(AS_MODE_SURROUND | AS_MODE_16CH);
	AS_SetDefaultSettings(AS_PCM_8BIT, 11025, AS_SURROUND);
	
	PA_OutputSimpleText(0, 1, 10, "Close the lid and see what     happens...");

	// Infinite loop to keep the program running
	while (1)
	{	
		PA_CloseLidSound2(Babye, HereWeGo); // Sound to play when lid closes and opens

		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()