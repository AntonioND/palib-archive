// Microphone example !

// Includes
#include <PA9.h>       // Include for PA_Lib

u8 MicData[100000]; // Array we will use to save the microphone...


// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	PA_InitText(0, 0);
	
	PA_InitSound();  // Init the sound system
	
	PA_OutputSimpleText(0, 4, 9, "Press A to start recording");
	PA_OutputSimpleText(0, 4, 10, "Press B to play sound");

	// Infinite loop to keep the program running
	while (1)
	{		// Record if A pressed... 
		if (Pad.Newpress.A) PA_MicStartRecording(MicData, 100000);  // Buffer, buffer length
		
		// If B, replay recorded sound
		if(Pad.Newpress.B) PA_MicReplay(0, MicData, 100000);   // Channel, buffer used, buffer length
			
		PA_OutputText(0, 2, 12, "Mic Volume : %d     ", PA_MicGetVol());  // Get mic volume, even if not recording

		PA_WaitForVBL();	
	}
	
	return 0;
} // End of main()
