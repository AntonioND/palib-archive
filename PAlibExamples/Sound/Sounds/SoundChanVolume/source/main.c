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
	
	PA_OutputSimpleText(0, 6, 10, "Press A to play the sound");
	PA_OutputSimpleText(0, 6, 11, "Up/Down to change volume");
		
	u8 volume = 127;

	// Infinite loop to keep the program running
	while (1)
	{		// Play the sound if A is pressed...
		if (Pad.Newpress.A) PA_PlaySimpleSound(0, saberoff);
		
		volume += Pad.Held.Up - Pad.Held.Down;
		volume &= 127;

		PA_OutputText(0, 6, 12, "Sound Volume : %03d", volume);
		PA_SetSoundChannelVol(0, volume); // Change current sound volume for a given channel (0 in this case). 
		// Sound volume limited to 0-127
			
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
