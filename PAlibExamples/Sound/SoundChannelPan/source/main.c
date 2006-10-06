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
	
	PA_OutputSimpleText(0, 4, 9, "Press A to play the sound");
	PA_OutputSimpleText(0, 4, 10, "Left/Right to change Pan");	
	
	u8 pan = 64;

	// Infinite loop to keep the program running
	while (1)
	{		// Play the sound if A is pressed...
		if (Pad.Newpress.A) PA_PlaySimpleSound(0, saberoff);
		
		pan += Pad.Held.Right - Pad.Held.Left;
		pan &= 127;

		PA_OutputText(0, 6, 12, "Channel Pan : %03d", pan);
		PA_SetSoundChannelPan(0, pan); // Change current sound volume for a given channel (0 in this case). 
		// Sound volume limited to 0-127
			
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
