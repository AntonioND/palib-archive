/* The text is drawn on a background (you can chose which background you want, 
and can display information on the screen
*/


// Includes
#include <PA9.h>       // Include for PA_Lib

#include "font/all_gfx.h"
#include "font/all_gfx.c"



// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	PA_InitText(0, 0);  // Initialise the normal text on the bottom screen

	// Load a custom text font
	PA_InitCustomText(1, //screen
					  0, //background number
					  newfont); //font name

	// PA_OutputSimpleText is the fastest text function, and displays 'static' text
	PA_OutputSimpleText(1, // screen
						2, // X postion
						2, // Y position
						"Hello World !!"); // 
						
	PA_OutputSimpleText(0, 2, 2, "Hi there :p");

	//PA_SetBgPalCol(1, 0, PA_RGB(31, 0,0)); // change the background color
	BG_PALETTE[768] = PA_RGB(31, 0,0);
	
	PA_OutputText(1, 1, 7, // Screen and position
				"Name : %s, Hour : %d, Float : %f3", // Write %s for string, %d for normal number, %f3 for float with 3 digits...
				PA_UserInfo.Name, PA_RTC.Hour, 1.25); // And then give the variables, same order
	PA_OutputText(1, 3, 10, "%04d", 12); // This will write 0012 instead of 12...
		
	
	
	// Infinite loop to keep the program running
	while (1)
	{

				
		// Get the stylus position and show it on screen
		PA_OutputText(1, 1, 11, "Stylus Position : %d, %d   ", Stylus.X, Stylus.Y);
		
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
