/* The text is drawn on a background (you can chose which background you want, 
and can display information on the screen
*/


// Includes
#include <PA9.h>       // Include for PA_Lib






// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	// Initialise the text system on the top screen
	PA_InitText(0, // Bootom screen
				0);  	// Background number, from 0-3, on which to load the text system
	PA_InitText(1, 0);  // Initialise the text system on the top screen

	
	// Infinite loop to keep the program running
	while (1)
	{
				
		// Get the stylus position and show it on screen
		PA_OutputText(1, 1, 11, "Stylus Position : %x, %x   ", Stylus.X, Stylus.Y);
		
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
