// Test to see if a channel is free or not

// Includes
#include <PA9.h>       // Include for PA_Lib
#include "saberoff.h"  // Include the sound (found in the data folder in .raw format)

// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	PA_InitText(0, 0);
	
	// Init the sound system
	AS_Init(AS_MODE_SURROUND | AS_MODE_16CH);
	AS_SetDefaultSettings(AS_PCM_8BIT, 11025, AS_SURROUND);
	
	u8 i;

	// Infinite loop to keep the program running
	while (1)
	{		
		if (Pad.Newpress.A) {
			PA_PlaySimpleSound(saberoff);
		}
		
		for (i = 0; i < 16; i++)
			PA_OutputText(0, 0, i, "Channel %d busy : %d  ", i, PA_SoundChannelIsBusy(i));	 // 0 for free, 1 for busy

		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
