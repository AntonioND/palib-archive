/* Shows how to use the FPS counter...
*/


// Includes
#include <PA9.h>       // Include for PA_Lib




// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	// Initialise the text system on the top screen
	PA_InitText(0, // Bootom screen
				0);  	// Background number, from 0-3, on which to load the text system


	s32 Times = 20;
	s32 i;
	float j, k;

	PA_OutputText(0, 2, 1, "Up/Down to change number of   times to execute code per VBL...The higher the number, the lowerthe FPS should be");
	
	// Infinite loop to keep the program running
	while (1)
	{
		Times += Pad.Held.Up - Pad.Held.Down;

		for (i = 0; i < Times*50; i++) { // Just a few divisions to stress the DS a little...
			k = i*20;
			PA_OutputText(0, 0, 23, "%d", k);
			PA_OutputText(0, 0, 23, "               ");	
			j = k*12/45;
			PA_OutputText(0, 0, 23, "%d", j);
			PA_OutputText(0, 0, 23, "               ");				
			k = j/i;
			PA_OutputText(0, 0, 23, "%d", k);
			PA_OutputText(0, 0, 23, "               ");	
		}
			
			
		
		
		PA_OutputText(0, 4, 6, "Executing %d times...     ", Times*50);
		
		PA_OutputText(0, 4, 8, "FPS : %02d ", PA_RTC.FPS);  // Display the frames per second
		
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
