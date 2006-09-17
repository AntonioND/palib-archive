// This example shows how to use 8 bit backgrounds, 
// on which you can draw with the stylus or show text in different sizes, colors, or rotations

// Includes
#include <PA9.h>       // Include for PA_Lib

#include <stdio.h>  // standard lib for texts...

char text[200];

// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	
	// Let's put some colors... in the background palettes
	PA_SetBgPalCol(0, 1, PA_RGB(31, 31, 31));

		PA_Init8bitBg(0, 3);


	// Infinite loop to keep the program running
	while (1)
	{
		sprintf(text,  // string to use
					"%02d/%02d/%02d", PA_RTC.Day, PA_RTC.Month, PA_RTC.Year);  // String to transform
		PA_SmartText(0, 10, 20, 255, 40, text, 1, 3, 0, 100);  // Display the text transformed :)

		// And the time...
		sprintf(text,  "%02d:%02d  %02d seconds", PA_RTC.Hour, PA_RTC.Minutes, PA_RTC.Seconds);
		PA_SmartText(0, 10, 40, 255, 60, text, 1, 3, 0, 100);	// Display the text transformed :)

		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
