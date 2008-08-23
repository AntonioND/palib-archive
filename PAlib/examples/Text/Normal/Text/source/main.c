/* The text is drawn on a background (you can chose which background you want, 
and can display information on the screen
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

	PA_SetTextCol(0, // Bottom screen
				0, // No red
				31, // Maximum green
				0); // No blue
	// This'll put a green color on the bottom screen... Color component (red, green, blue) range from 0 to 31

	// PA_OutputSimpleText is the fastest text function, and displays 'static' text
	PA_OutputSimpleText(1, // screen
						2, // X postion
						2, // Y position
						"Hello World !!"); // 
						
	PA_OutputSimpleText(0, 2, 2, "Hi there :p");
	
	// By the way, to erase text, just right "     " (spaces) over it... 
	
	
	// There are some advance text function : PA_OutputText
	// Let's use it to display a string (username), a number (current hour), and a float (1.25) :
	PA_OutputText(1, 1, 7, // Screen and position
				"Name : %s, Hour : %d, Float : %f3", // Write %s for string, %d for normal number, %f3 for float with 3 digits...
				PA_UserInfo.Name, PA_RTC.Hour, 1.25); // And then give the variables, same order
				
	// You can also use %0Xd, X being a number, to complete the number by zeros :
	PA_OutputText(1, 3, 10, "%04d", 12); // This will write 0012 instead of 12...
	
	
	
	// Infinite loop to keep the program running
	while (1)
	{

				
		// Get the stylus position and show it on screen
		PA_OutputText(1, 1, 11, "Stylus Position : %d, %d   ", Stylus.X, Stylus.Y);
		
		PA_WaitForVBL();
		PA_OutputText(0, 0, 0, "%d   ", PA_GetVcount());
	}
	
	return 0;
} // End of main()
