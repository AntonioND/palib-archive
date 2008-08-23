/* This time, draw the text letter by letter...
*/


// Includes
#include <PA9.h>       // Include for PA_Lib


char text[200]; // This will be our text...



// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	// Initialise the text system on the top screen
	PA_InitText(0, // Bootom screen
				0);  	// Background number, from 0-3, on which to load the text system
	PA_InitText(1, 0);  // Initialise the text system on the top screen

	// This'll put a green color on the bottom screen... Color component (red, green, blue) range from 0 to 31

	u32 nletters = 0; // Number of letter to output...
	u32 letter = 0; // Current letter written...
	
	while (letter == nletters){ // Do this until the function outputs all the text
		++nletters; // Next frame, one more letter...	
		// This text function outputs a given number of letters... This way, it can do as if you were typing !
		letter = PA_BoxText(1, 2, 2, 29, 15, "Hi there :p  Cool, the function works perfectly ! So you see the %c1text%c0 being typed...\n Line break works...", nletters); //%cX changes to color number X
		PA_WaitForVBL(); // You can set more WaitForVBL if you want to slow down the text output...
	}

	PA_OutputSimpleText(0, 10, 10, "Finished !"); // Finished...


	// Infinite loop to keep the program running
	while (1){

		PA_WaitForVBL();
		
	}
	
	return 0;
} // End of main()
