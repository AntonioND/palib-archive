// Includes
#include <PA9.h>

// Include the sound
#include "sfx.h"

int main(int argc, char** argv)
{
	// PALib Initialization
	PA_Init();
	PA_InitVBL();
	PA_InitText(0, 0);

	// Init AS_Lib for normal sound playback only
	AS_Init(AS_MODE_SURROUND | AS_MODE_16CH);
	AS_SetDefaultSettings(AS_PCM_8BIT, 11025, AS_SURROUND);
	
	// Display info text
	PA_OutputText(0, 1, 1, "Simple sound playback example\n\nUse A to start the sound");
	
	// Infinite loop to keep the program running
	while (1)
	{
		// Play sound when the user presses A
        if (Pad.Newpress.A)
		{
            AS_SoundQuickPlay(sfx); // This returns the channel the sound is played on so you can apply effects like changing the pan or volume afterwards.
        }

        PA_WaitForVBL();
    }
	
	return 0;
}
