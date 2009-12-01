// Includes
#include <PA9.h>       // Include for PA_Lib


// Function: main()
int main()
{
	PA_Init();    // Initializes PA_Lib
	
	PA_LoadDefaultText(0,0);
	PA_LoadDefaultText(1,0);
	
	PA_OutputText(0, 4, 1, "Text on the bottom screen!");
	PA_OutputText(1, 5, 1, "Text on the top screen!");
	
	// Infinite loop to keep the program running
	while (1)
	{
		if (Pad.Newpress.A) PA_SetScreenLight(0, // screen
												1); // Turn on bottom light
		if (Pad.Newpress.B) PA_SetScreenLight(0, 0);
		if (Pad.Newpress.X) PA_SetScreenLight(1, 1);// Turn on top light
		if (Pad.Newpress.Y) PA_SetScreenLight(1, 0); 
		
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()