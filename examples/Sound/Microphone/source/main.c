// Microphone example

// Includes
#include <PA9.h>       // Include for PA_Lib

#define SEC_TO_LENGTH(sec) ((sec) * 16384)
#define BUF_SIZE SEC_TO_LENGTH(10) // ten seconds

u8 MicData[BUF_SIZE]; // Array we will use to save the microphone...

// Function: main()
int main()
{
	PA_Init();    // Initializes PA_Lib
	PA_LoadDefaultText(0, 0);
	
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
			PA_MicStartRecording(MicData, BUF_SIZE);  // Buffer, buffer length
		
		// If B, replay recorded sound
		if(Pad.Newpress.B)
			PA_MicReplay(MicData, BUF_SIZE);   // Buffer used, buffer length
		
		// Get mic volume, even if not recording
		// Notes about mic volume:
		// It is calculated using a special algorithm made by fincs.
		// He tested it on his NDS and discovered the following values:
		// 0 - ~75 = background noise
		// ~75 - ~150 = regular talking
		// ~150 - 255 = blowing into the mic
		// The algorithm:
		// f(x) = (cnv(x) + old) / 2, old = cnv(x)
		// cnv(x) = abs(x) * 2
		PA_OutputText(0, 2, 12, "Mic Volume: %d     ", PA_MicGetVol());
		PA_OutputSimpleText(0, 2, 13, PA_MicGetVol() > 150 ? "Blowing!" : "        ");

		PA_WaitForVBL();	
	}
	
	return 0;
}