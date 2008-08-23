// Includes
#include <PA9.h>       // Include for PA_Lib


int C_scale[8] = {33,37,41,44,49,55,62,65}; // Scale for notes in C major scale

// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	// Init the sound system
	AS_Init(AS_MODE_SURROUND | AS_MODE_16CH);
	AS_SetDefaultSettings(AS_PCM_8BIT, 11025, AS_SURROUND);
	
	PA_InitText(1, 0);  // Initialise the text system on the top screen

	u8 pan;
	s32 note;
	u8 duty;
	
	pan = 64;
	
	PA_OutputText(1, 1, 9,"Use stylus to change note/duty, and keys to change pan");
	
	// Infinite loop to keep the program running
	while (1)
	{
		
		note = C_scale[(Stylus.X>>5)]<<4;
		//pan = (Stylus.Y*0.66);
		pan += Pad.Held.Right-Pad.Held.Left; // Change pan with pad
		duty = Stylus.Y/24; // 0-7


		// Check the stylus presses :
		if (Stylus.Held){
		   PA_PlayPSG(8, 127, pan, note, duty);
		   PA_PlayPSG(9, 127, pan, note*2, duty);//octave
			 }
		else{
		    PA_PlayPSG(8, 0, 0, 0, 0);  // Blank sound
			 PA_PlayPSG(9, 0, 0, 0, 0);  // Blank sound
			}			 

				
		// Get the stylus position and show it on screen
		PA_OutputText(1, 1, 11, "Stylus Position : %d, %d   ", Stylus.X, Stylus.Y);
		PA_OutputText(1, 1, 12, "Note : %d  Pan : %d  Duty : %d    ", note, pan, duty);		

		
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
