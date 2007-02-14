/* DS Motion Example !!
*/


// Includes
#include <PA9.h>       // Include for PA_Lib




// global variable indicating whether DS Motion Card is plugged in
int ndsmotion = 0;


// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	PA_InitText(0, 0);  // Initialise the text system on the top screen	
	PA_InitText(1, 0);  // Initialise the text system on the top screen

	PA_OutputSimpleText(1, 0, 5, "Please insert your DS Motion and press any key"); // Ask to insert the DS Motion...

	PA_WaitFor(Pad.Newpress.Anykey);  // Wait for any key to be pressed before continuing...
	PA_ClearTextBg(1);		// Erase text screen...
	
	 // Turn on the DS Motion...
	 PA_MotionInit();
	 
	while ( !PA_CheckDSMotion()) { // Checks for the DS Motion, continues looking for it until it actually finds it...
		PA_WaitForVBL();
		
		// If no DSMotion found...
		PA_OutputText(1, 0, 5, "      DS Motion not found ! \nPlease insert your DS Motion and press any key"); // Ask to insert the DS Motion... again...
		
		PA_WaitFor(Pad.Newpress.Anykey);  // Wait for any key to be pressed before continuing...
		PA_MotionInit();
	}
	
	PA_ClearTextBg(1); // erase the text...
	
	PA_OutputText(1, 0, 5, "Move to draw, simple but effective ^^");
	PA_OutputText(1, 0, 5, "Press any key to erase...");	
	PA_ResetBgSysScreen(0);
	
	s16 x = 128;
	s16 y = 96;
	
	PA_Init8bitBg(0, 0);
	PA_SetBgPalCol(0, 0, PA_RGB(31, 31, 31));
	PA_SetBgPalCol(0, 1, 0);
	
	// Infinite loop to keep the program running
	while (1)
	{
	   
	   x += Motion.Held.Right - Motion.Held.Left;
	   y += Motion.Held.Down - Motion.Held.Up;
	   if(x < 0) x = 0; if (x > 255) x = 255;
	   if(y < 0) y = 0; if (y > 191) y = 191;
	   
	   PA_Put8bitPixel(0, x, y, 1); // Plot a pixel..
	   
	   if(Pad.Newpress.Anykey){
	      x = 128;
	      y = 96;
	      PA_Clear8bitBg(0); // Erase all...
		}   
	
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()


