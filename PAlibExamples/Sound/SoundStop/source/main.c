// Sound example ! Add the sound raw file 

// Includes
#include <PA9.h>       // Include for PA_Lib
#include "saberoff.h"  // Include the sound (found in the data folder in .raw format)

// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	PA_InitText(0, 0);
	
	
	PA_InitSound();  // Init the sound system
	
	PA_OutputSimpleText(0, 6, 10, "Press A to play/stop the sound");

	// Infinite loop to keep the program running
	while (1)
	{		
		if (Pad.Newpress.A) {  // When A is pressed...
			if(PA_SoundChannelIsBusy(0)) PA_StopSound(0); // If busy, stop the sound
			else PA_PlaySimpleSound(0, saberoff);  	// If not, reload it :)
		}
		
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
