/* Text in different colors...
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
				1);  	// Background number, from 0-3, on which to load the text system
	PA_InitText(1, 0);  // Initialise the text system on the top screen

	u8 i;
	for (i = 0; i < 10; i++)
	{
		PA_SetTextTileCol(1, i); // Change the color on the top screen, values 0 to 9 (9 is black, so invisible on black background...)
		PA_OutputSimpleText(1, 2, i, "Color test...");  // Screen 1 has different colors
	}
	
	// You can also use macros TEXT_WHITE, TEXT_RED, etc...	
	PA_SetTextTileCol(1, TEXT_YELLOW); // Change the color on the top screen to yellow
   PA_OutputSimpleText(1, 0, 17, "Yellow test..."); 	
	
	// You can also change the text color in a given text (either in OutputText or BoxText) :
	PA_OutputText(0, 0, 0, "Color test...%c1another one, %c2again, %c3 again...");  // Screen 1 has different colors
	//%c tells to change color, and the number gives the color number (0-7)

	
	// Infinite loop to keep the program running
	while (1)
	{
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
