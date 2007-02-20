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
	
	// Infinite loop to keep the program running
	while (1)
	{
	   
	   while(!PA_CheckDSMotion()){ // Function that checks if the DS Motion is still present...
	   	PA_OutputText(1, 0, 5, "Please reinsert DSMotion..."); // Ask to insert the DS Motion... again...
	   	PA_WaitForVBL();
	 	}  	
	 	PA_ClearTextBg(1); // Clear bottom screen...

		PA_OutputText(1, 3, 5, "Motion.X     : %d     ", Motion.X);
		PA_OutputText(1, 3, 6, "Motion.Vx    : %d     ", Motion.Vx);		
		PA_OutputText(1, 3, 7, "Motion.Y     : %d     ", Motion.Y);	
		PA_OutputText(1, 3, 8, "Motion.Vy    : %d     ", Motion.Vy);			
		PA_OutputText(1, 3, 9, "Motion.Z     : %d     ", Motion.Z);
		PA_OutputText(1, 3, 10, "Motion.Vz    : %d     ", Motion.Vz);		
		PA_OutputText(1, 3, 11, "Motion.Zrot  : %d     ", Motion.Zrot);
		PA_OutputText(1, 3, 12, "Motion.Vzrot : %d     ", Motion.Vzrot);
		
		PA_OutputText(1, 3, 14, "Motion.Angle : %d     ", Motion.Angle);
		PA_OutputText(1, 3, 15, "Motion.Force : %d     ", Motion.Force);	
		
		PA_OutputText(0, 3, 7, "Up     : %d-%d-%d", Motion.Newpress.Up, Motion.Held.Up, Motion.Released.Up);
		PA_OutputText(0, 3, 8, "Down   : %d-%d-%d", Motion.Newpress.Down, Motion.Held.Down, Motion.Released.Down);
		PA_OutputText(0, 3, 9, "Left   : %d-%d-%d", Motion.Newpress.Left, Motion.Held.Left, Motion.Released.Left);		
		PA_OutputText(0, 3, 10, "Right  : %d-%d-%d", Motion.Newpress.Right, Motion.Held.Right, Motion.Released.Right);	

	
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()


