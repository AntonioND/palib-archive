/* This will be the shortest example ever ! It just turns of the scren when the lid is closed...
Therefore, usable only on DS, duh...
*/


// Includes
#include <PA9.h>       // Include for PA_Lib


// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	PA_SetBgPalCol(0, 0, PA_RGB(31, 0, 0)); // Set the background color to red, to check if on...
	
	// Infinite loop to keep the program running
	while (1)
	{
		PA_CheckLid(); // Checks the lid, and pauses if closed... Returns 1 when it unpauses
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
