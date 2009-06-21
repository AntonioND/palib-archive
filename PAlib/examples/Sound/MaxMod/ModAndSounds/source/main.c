/*
This example shows the use of the MaxMod library by eKid.

It comes with devkitPro, so using it doesn't require any installation.

Just be sure the Makefile for your project has ARM7_MAXMOD_DSWIFI
as the selected ARM7 binary.
*/

// Includes
#include <PA9.h>
#include <maxmod9.h> // MaxMod include

// Include the soundbank
#include "soundbank_bin.h"
#include "soundbank.h"

int main(int argc, char** argv)
{
	// PALib Initialization
	PA_Init();
	PA_InitVBL();
	PA_InitText(0, 0);

	// Initialize MaxMod
	mmInitDefaultMem((mm_addr)soundbank_bin);

	// Load and play the music
	mmLoad(MOD_MUSIC);
	mmStart(MOD_MUSIC, MM_PLAY_LOOP);

	// Load the sounds
	// Please note that they are regular .wav
	// files put into the music folder
	mmLoadEffect(SFX_SFXA);
	mmLoadEffect(SFX_SFXB);

	// Display info text
	PA_OutputText(0, 1, 1, "MaxMod example:\nplaying music and sounds\n\nA - play sound effect #1\nB - play sound effect #2");
	
	// Infinite loop to keep the program running
	while (1)
	{
		// Play the sounds when the user presses A or B
        if (Pad.Newpress.A)
		{
			mmEffect(SFX_SFXA);
        }
		if (Pad.Newpress.B)
		{
			mmEffect(SFX_SFXB);
		}

        PA_WaitForVBL();
    }
	
	return 0;
}
