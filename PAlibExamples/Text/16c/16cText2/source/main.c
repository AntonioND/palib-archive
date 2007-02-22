// Includes
#include <PA9.h>       // Include for PA_Lib

#include <stdio.h>  // standard lib for texts...

char text[200];

// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	

	
	PA_Init16cBg(0, 3); 	PA_Init16cBg(1, 3);  // 16 color background init with default colors for text
	


	
	// Infinite loop to keep the program running
	while (1)
	{
		sprintf(text,  // string to use
					"%02d/%02d/%02d", PA_RTC.Day, PA_RTC.Month, PA_RTC.Year);  // String to transform
		PA_16cText(0, 10, 20, 255, 40, text, 1, 3, 100);  // Display the text transformed :)

	// And the time...
		sprintf(text,  "%02d:%02d  %02d seconds", PA_RTC.Hour, PA_RTC.Minutes, PA_RTC.Seconds);
		PA_16cText(0, 10, 40, 255, 60, text, 1, 3, 100);	// Display the text transformed :)

		PA_WaitForVBL();
		PA_16cErase(0);
	}
	
	return 0;
} // End of main()
