/* Here you'll see all the pad information, which is really not much :
- Each key has 3 variables : Newpress, Held, and Released
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

	
	PA_OutputSimpleText(1, 0, 3, "For each key, we'll show Newpress, Held, and Released");
	
	
	
	// Infinite loop to keep the program running
	while (1)
	{
	// For each key, we'll show Newpress, Held, and Released
		PA_OutputText(1, 3, 7, "Up     : %d-%d-%d", Pad.Newpress.Up, Pad.Held.Up, Pad.Released.Up);
		PA_OutputText(1, 3, 8, "Down   : %d-%d-%d", Pad.Newpress.Down, Pad.Held.Down, Pad.Released.Down);
		PA_OutputText(1, 3, 9, "Left   : %d-%d-%d", Pad.Newpress.Left, Pad.Held.Left, Pad.Released.Left);		
		PA_OutputText(1, 3, 10, "Right  : %d-%d-%d", Pad.Newpress.Right, Pad.Held.Right, Pad.Released.Right);		
		PA_OutputText(1, 3, 11, "A      : %d-%d-%d", Pad.Newpress.A, Pad.Held.A, Pad.Released.A);		
		PA_OutputText(1, 3, 12, "B      : %d-%d-%d", Pad.Newpress.B, Pad.Held.B, Pad.Released.B);		
		PA_OutputText(1, 3, 13, "X      : %d-%d-%d", Pad.Newpress.X, Pad.Held.X, Pad.Released.X);		
		PA_OutputText(1, 3, 14, "Y      : %d-%d-%d", Pad.Newpress.Y, Pad.Held.Y, Pad.Released.Y);		
		PA_OutputText(1, 3, 15, "L      : %d-%d-%d", Pad.Newpress.L, Pad.Held.L, Pad.Released.L);		
		PA_OutputText(1, 3, 16, "R      : %d-%d-%d", Pad.Newpress.R, Pad.Held.R, Pad.Released.R);
		PA_OutputText(1, 3, 17, "Start  : %d-%d-%d", Pad.Newpress.Start, Pad.Held.Start, Pad.Released.Start);
		PA_OutputText(1, 3, 18, "Select : %d-%d-%d", Pad.Newpress.Select, Pad.Held.Select, Pad.Released.Select);
		
		PA_OutputText(1, 3, 20, "Anykey : %d-%d-%d", Pad.Newpress.Anykey, Pad.Held.Anykey, Pad.Released.Anykey); // Any key !
		
	
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
