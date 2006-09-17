/* This code can be used to compare the speed of one or more functions !
*/


// Includes
#include <PA9.h>       // Include for PA_Lib




// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	PA_InitText(0, 0);  

				
	PA_TestInit(0, // screen on which to display the text...
					0.5); // number of seconds to test each function.. you can use 0.5 if you don't want to wait more ^^
				
	s32 ypos = 0;	
	
	PA_TESTSTART   // Indicates the start of the test, do not put anything but PA_TESTSTART on this line
		ypos += Pad.Held.Up - Pad.Held.Down; // Function to test, just one line here, but can be longer... 
		
		
	PA_TESTEND  // End of the function, nothing more on this line...
	
	
	
	PA_TESTSTART  // Restart over and over for other functions
		if(Pad.Held.Up) ypos ++;
		else if (Pad.Held.Down) ypos --;
		// different way of doing the same thing, to see which is actually the fastest...
		
	PA_TESTEND
	
				
	PA_TestResults(); // Once finished, this will display the rsults
	
	
	// Infinite loop to keep the program running
	while (1)
	{


		
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
