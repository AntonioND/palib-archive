// Microphone example

// Includes
#include <PA9.h>       // Include for PA_Lib

u8 MicData[100000]; // Array we will use to save the microphone...

// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	PA_InitText(0, 0);
	
	// Init the sound system
	AS_Init(AS_MODE_SURROUND | AS_MODE_16CH);
	AS_SetDefaultSettings(AS_PCM_8BIT, 11025, AS_SURROUND);
	
	PA_OutputSimpleText(0, 4, 9, "Press A to start recording");
	PA_OutputSimpleText(0, 4, 10, "Press B to play sound");

	// Infinite loop to keep the program running
	while (1)
	{
		// Record if A pressed... 
		if (Pad.Newpress.A)
			PA_MicStartRecording(MicData, 100000);  // Buffer, buffer length
		
		// If B, replay recorded sound
		if(Pad.Newpress.B)
			PA_MicReplay(MicData, 100000);   // Buffer used, buffer length
		
		// Get mic volume, even if not recording
		PA_OutputText(0, 2, 12, "Mic Volume : %d     ", PA_MicGetVol());

		PA_WaitForVBL();	
	}
	
	return 0;
}
