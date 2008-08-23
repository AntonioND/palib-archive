// This is basicly an example using the brightness to fade in/out...



// Includes
#include <PA9.h>       // Include for PA_Lib



#include "gfx/all_gfx.h"
#include "gfx/all_gfx.c"




// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	PA_InitText(1, 0);
	
	PA_EasyBgLoad(0, 1, pasplash);	

	s8 i;
	
	// Infinite loop to keep the program running
	while (1)
	{
	
		PA_OutputText(1, 8, 9, "Fading to White...   ");
		// Fade out to white
		for (i = 0; i <= 31; i++) {
			PA_SetBrightness(0, // Screen
									i); // Brightness, 0 for normal, 31 for white, -31 for black, intermediates possible...
			PA_WaitForVBL(); // To slow down the fades, we wait a frame...
			PA_WaitForVBL(); 
		}	
		
		for(i = 0; i < 32; i++) PA_WaitForVBL(); // Wait a litte...
		
		PA_OutputText(1, 8, 9, "Back to normal...   ");		
		
		// Fade back in from white
		for (i = 31; i >= 0; i--) {
			PA_SetBrightness(0, i);
			PA_WaitForVBL(); // To slow down the fades, we wait a frame...
			PA_WaitForVBL(); 
		}		
		
		for(i = 0; i < 32; i++) PA_WaitForVBL(); // Wait a litte...

		PA_OutputText(1, 8, 9, "Fading to Black...     ");						
		// Fade out to black
		for (i = 0; i >= -31; i--) {
			PA_SetBrightness(0, i);
			PA_WaitForVBL(); // To slow down the fades, we wait a frame...
			PA_WaitForVBL(); 
		}	
		
		for(i = 0; i < 32; i++) PA_WaitForVBL(); // Wait a litte...
		
		PA_OutputText(1, 8, 9, "Back to normal...   ");
		// Back in from black...
		for (i = -31; i <= 0; i++) {
			PA_SetBrightness(0, i);
			PA_WaitForVBL(); // To slow down the fades, we wait a frame...
			PA_WaitForVBL(); 
		}	

		for(i = 0; i < 32; i++) PA_WaitForVBL(); // Wait a litte...
	}
	
	return 0;
} // End of main()
