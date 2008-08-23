/*
This example shows the use of the simple MOD player by hitchhikr instead
of the full MikMod library.

hitchhikr's MOD player is included in PAlib by default, so using it doesn't
require any installation.

Just be sure the Makefile for your project has ARM7_MOD_DSWIFI or
ARM7_MOD_LIBLOBBY as the selected ARM7 binary.
*/


// Includes
#include <PA9.h>

// Include the music
#include "music.h"

int main(int argc, char** argv)
{
	// PALib Initialization
	PA_Init();
	PA_InitVBL();
	PA_InitText(0, 0);

	// Display info text
	PA_OutputText(0, 1, 1, "Simple mod playback example\n\nUse A to start the music,\nB to stop it");
	
	// Infinite loop to keep the program running
	while (1)
	{
		// Play sound when the user presses A
        if (Pad.Newpress.A)
		{
			PA_PlayMod(music);
        }
		else if (Pad.Newpress.B)
		{
			PA_StopMod();
		}

        PA_WaitForVBL();
    }
	
	return 0;
}
