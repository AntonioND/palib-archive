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
	
	PA_InitSound();  // Init the sound system
	
	u8 channel = 0;
	u8 i;

	// Infinite loop to keep the program running
	while (1)
	{		
		if (Pad.Newpress.A) {
			PA_PlaySimpleSound(channel, saberoff);
			channel++; channel&=15; // limit to 0-15
		}
		
		for (i = 0; i < 16; i++) PA_OutputText(0, 0, i, "Channel %d busy : %d  ", i, PA_SoundChannelIsBusy(i));	 // 0 for free, 1 for busy
		
		PA_OutputText(0, 0, 20, "Free Channel : %d    ", PA_GetFreeSoundChannel());  // First free channel

		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
