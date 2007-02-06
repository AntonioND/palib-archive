/* Print function, usefull for debugging purpose...
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

	PA_Print(0, "Hello World %d  \n", pa_printscroll[0]);					
	PA_Print(0, "Hi there ! :p\n");

	
	// Infinite loop to keep the program running
	while (1)
	{
				
		// Get the stylus position and show it on screen when the stylus touches the screen...
		if(Stylus.Held) PA_Print(0, "Stylus Position : %d, %d   \n", Stylus.X, Stylus.Y);
		
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
