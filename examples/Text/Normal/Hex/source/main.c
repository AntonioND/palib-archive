/* The text is drawn on a background (you can chose which background you want, 
and can display information on the screen
*/


// Includes
#include <PA9.h>       // Include for PA_Lib






// Function: main()
int main()
{
	PA_Init();    // Initializes PA_Lib
	
	// Initialise the text system on the top screen
	PA_LoadDefaultText(0, // Bootom screen
				0);  	// Background number, from 0-3, on which to load the text system
	PA_LoadDefaultText(1, 0);  // Initialise the text system on the top screen

	
	// Infinite loop to keep the program running
	while (1)
	{
				
		// Get the stylus position and show it on screen
		PA_OutputText(1, 1, 11, "Stylus Position : %x, %x   ", Stylus.X, Stylus.Y);
		
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()