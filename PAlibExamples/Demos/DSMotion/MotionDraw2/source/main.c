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
	
	s32 x = 128<<8;
	s32 y = 96<<8;
	s32 oldx, oldy;
	
	PA_Init8bitBg(0, 0);
	PA_SetBgPalCol(0, 0, PA_RGB(31, 31, 31));
	PA_SetBgPalCol(0, 1, 0);
	
	PA_SetDrawSize(0, 1);
	
	// Infinite loop to keep the program running
	while (1)
	{
	   // Backup old coordinates
	   oldx = x;
	   oldy = y;
  
	   x -= Motion.X<<3;
	   y += Motion.Y<<3;
	   if(x < 0) x = 0; if (x > 255<<8) x = 255<<8;
	   if(y < 0) y = 0; if (y > 191<<8) y = 191<<8;
	   
	   PA_Draw8bitLine(0, oldx>>8, oldy>>8, x>>8, y>>8, 1);
	   
	   if(Pad.Newpress.Anykey){
			x = 128<<8;
			y = 96<<8;
	      PA_Clear8bitBg(0); // Erase all...
		}   
	
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()


