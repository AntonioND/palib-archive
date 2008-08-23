/* Here you'll see all the stylus information, which is really not much :
- Stylus location
- Stylus newly pressed, held, or released...
*/


// Includes
#include <PA9.h>       // Include for PA_Lib


char text[200]; // This will be our text...



// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	
	PA_InitText(1, 0);  // Initialise the text system on the top screen

	
	u8 i;
	
	// Infinite loop to keep the program running
	while (1)
	{
		// Erase the text on the screen...
		for (i = 0; i < 10; i++) PA_OutputSimpleText(1, 6, 7+i, "                              ");
		
		// Check the stylus presses :
		if (Stylus.Held) PA_OutputSimpleText(1, 6, 7, "Stylus is held !");
		if (Stylus.Newpress) PA_OutputSimpleText(1, 6, 8, "Stylus is newly pressed !");
		if (Stylus.Released) PA_OutputSimpleText(1, 6, 9, "Stylus is just released !");
		if (Stylus.DblClick) PA_OutputSimpleText(1, 6, 10, "Stylus double clicked !");		
		
		PA_OutputText(1, 1, 11, "Stylus held down for %d seconds   ", Stylus.Downtime/60);  // Stylus.Downtime/Uptime in VBL time, 60/second
		PA_OutputText(1, 1, 12, "Stylus kept up for   %d seconds   ", Stylus.Uptime/60);		
				
		// Get the stylus position and show it on screen
		PA_OutputText(1, 1, 14, "Stylus Position : %d, %d   ", Stylus.X, Stylus.Y);
		
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
