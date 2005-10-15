/* API system, example : Checkboxes...
Checkboxes are just small boxes, 8x8.
When you click on them, they fill with a check, and the variable you associated to it goes from 0 to 1 !
And if you click again, removes the check and put the value back to 0...

*/

// Includes
#include <PA9.h>       // Include for PA_Lib



// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	PA_InitText(1, 0); PA_SetTextCol(1, 31, 0, 0); // Red text
	
	// Init 8 bit backgrounds on both screens
	PA_Init8bitBg(0, 3);
	PAPI_Init(0); // Init the API on the bottom screen
	
	bool test = 0; bool test2 = 0; // We'll check if the variables change automatically or not

	PAPI_CreateCheck(0, // screen
					30, // x position
					30, // y position
					&test); // variable to change, must be of bool type
	PAPI_CreateCheck(0, 50, 30, &test2);
	
	// Infinite loop to keep the program running
	while (1)
	{
		PAPI_CheckCheck(); // Checks if the checks are pressed...
		
		// And now we'll see if it works
		PA_OutputText(1, 0, 0, "test : %d  \ntest2 : %d  ", test, test2);
		
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
