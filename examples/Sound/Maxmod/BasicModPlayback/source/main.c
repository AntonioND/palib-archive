/*
This example shows the basic usage of the Maxmod library by eKid.

It comes with libnds, so using it doesn't require any installation.

Just be sure the Makefile for your project has ARM7_MAXMOD_DSWIFI
as the selected ARM7 binary.
*/

// Includes
#include <PA9.h>
#include <maxmod9.h> // Include Maxmod

// Include the soundbank
#include "soundbank_bin.h"
#include "soundbank.h"

int main(){
	// PAlib Initialization
	PA_Init();
	PA_LoadDefaultText(0, 0);

	// Initialize Maxmod
	mmInitDefaultMem((mm_addr)soundbank_bin);

	// Load the music
	mmLoad(MOD_MUSIC);

	// Display info text
	PA_OutputText(0, 1, 1, "Simple Maxmod example\n\nUse A to start the music,\nB to stop it");

	// Infinite loop to keep the program running
	while(true){
		// Play the module when the user presses A
        if(Pad.Newpress.A)
			mmStart(MOD_MUSIC, MM_PLAY_LOOP);
		else if(Pad.Newpress.B)
			mmStop();

        PA_WaitForVBL();
    }
}