/* Here you'll see all the pad information, which is really not much :
- Each key has 3 variables : Newpress, Held, and Released
*/


// Includes
#include <PA9.h>       // Include for PA_Lib


// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	PA_SetAutoUpdatePadTimes(1); // Call this if you want to use Uptime, Downtime or DblClick
	
	PA_InitText(1, 0);
	PA_InitText(0, 0);

	PA_OutputSimpleText(1, 0, 3, "For each key, we'll show\nNewpress, Held, and Released");
	PA_OutputSimpleText(0, 0, 3, "For each key, we'll show\nDblClick, Uptime and Downtime");
	
	// Infinite loop to keep the program running
	while (1)
	{
		// For each key, we'll show Newpress, Held, and Released
		PA_OutputText(1, 3,  7, "Up     : %d-%d-%d", Pad.Newpress.Up,     Pad.Held.Up,     Pad.Released.Up);
		PA_OutputText(1, 3,  8, "Down   : %d-%d-%d", Pad.Newpress.Down,   Pad.Held.Down,   Pad.Released.Down);
		PA_OutputText(1, 3,  9, "Left   : %d-%d-%d", Pad.Newpress.Left,   Pad.Held.Left,   Pad.Released.Left);
		PA_OutputText(1, 3, 10, "Right  : %d-%d-%d", Pad.Newpress.Right,  Pad.Held.Right,  Pad.Released.Right);
		PA_OutputText(1, 3, 11, "A      : %d-%d-%d", Pad.Newpress.A,      Pad.Held.A,      Pad.Released.A);
		PA_OutputText(1, 3, 12, "B      : %d-%d-%d", Pad.Newpress.B,      Pad.Held.B,      Pad.Released.B);
		PA_OutputText(1, 3, 13, "X      : %d-%d-%d", Pad.Newpress.X,      Pad.Held.X,      Pad.Released.X);
		PA_OutputText(1, 3, 14, "Y      : %d-%d-%d", Pad.Newpress.Y,      Pad.Held.Y,      Pad.Released.Y);
		PA_OutputText(1, 3, 15, "L      : %d-%d-%d", Pad.Newpress.L,      Pad.Held.L,      Pad.Released.L);
		PA_OutputText(1, 3, 16, "R      : %d-%d-%d", Pad.Newpress.R,      Pad.Held.R,      Pad.Released.R);
		PA_OutputText(1, 3, 17, "Start  : %d-%d-%d", Pad.Newpress.Start,  Pad.Held.Start,  Pad.Released.Start);
		PA_OutputText(1, 3, 18, "Select : %d-%d-%d", Pad.Newpress.Select, Pad.Held.Select, Pad.Released.Select);
		PA_OutputText(1, 3, 20, "Anykey : %d-%d-%d", Pad.Newpress.Anykey, Pad.Held.Anykey, Pad.Released.Anykey);
		
		// For each key, we'll show DblClick, Uptime and Downtime
		PA_OutputText(0, 3,  7, "Up     : %d-%d-%d         ", Pad.DblClick.Up,     Pad.Uptime.Up,     Pad.Downtime.Up);
		PA_OutputText(0, 3,  8, "Down   : %d-%d-%d         ", Pad.DblClick.Down,   Pad.Uptime.Down,   Pad.Downtime.Down);
		PA_OutputText(0, 3,  9, "Left   : %d-%d-%d         ", Pad.DblClick.Left,   Pad.Uptime.Left,   Pad.Downtime.Left);
		PA_OutputText(0, 3, 10, "Right  : %d-%d-%d         ", Pad.DblClick.Right,  Pad.Uptime.Right,  Pad.Downtime.Right);
		PA_OutputText(0, 3, 11, "A      : %d-%d-%d         ", Pad.DblClick.A,      Pad.Uptime.A,      Pad.Downtime.A);
		PA_OutputText(0, 3, 12, "B      : %d-%d-%d         ", Pad.DblClick.B,      Pad.Uptime.B,      Pad.Downtime.B);
		PA_OutputText(0, 3, 13, "X      : %d-%d-%d         ", Pad.DblClick.X,      Pad.Uptime.X,      Pad.Downtime.X);
		PA_OutputText(0, 3, 14, "Y      : %d-%d-%d         ", Pad.DblClick.Y,      Pad.Uptime.Y,      Pad.Downtime.Y);
		PA_OutputText(0, 3, 15, "L      : %d-%d-%d         ", Pad.DblClick.L,      Pad.Uptime.L,      Pad.Downtime.L);
		PA_OutputText(0, 3, 16, "R      : %d-%d-%d         ", Pad.DblClick.R,      Pad.Uptime.R,      Pad.Downtime.R);
		PA_OutputText(0, 3, 17, "Start  : %d-%d-%d         ", Pad.DblClick.Start,  Pad.Uptime.Start,  Pad.Downtime.Start);
		PA_OutputText(0, 3, 18, "Select : %d-%d-%d         ", Pad.DblClick.Select, Pad.Uptime.Select, Pad.Downtime.Select);
		PA_OutputText(0, 3, 20, "Anykey : %d-%d-%d         ", Pad.DblClick.Anykey, Pad.Uptime.Anykey, Pad.Downtime.Anykey);
	
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
