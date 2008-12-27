// A simple demo of using the Guitar Hero Grip with PAlib
// by BassAceGold


// Includes
#include <PA9.h>       // Include for PA_Lib


// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	PA_InitGHPad(); // Call this first before using other GH controller functions

	PA_SetAutoUpdateGHPadTimes(1); // Call this if you want to use Uptime, Downtime or DblClick

	PA_InitText(1,0);
	PA_InitText(0,0);

	PA_OutputSimpleText(1, 0, 0, "GH Button Test");
	PA_OutputSimpleText(1, 0, 1, "by BassAceGold");
	PA_OutputSimpleText(1, 0, 3, "For each key, we'll show\nNewpress, Held, and Released");
	PA_OutputSimpleText(0, 0, 3, "For each key, we'll show\nDblClick, Uptime and Downtime");

	// Infinite loop to keep the program running
	while (1)
	{
		// For each key, we'll show Newpress, Held, and Released
		PA_OutputText(1, 3,  7, "Green  : %d-%d-%d", GHPad.Newpress.Green,  GHPad.Held.Green,  GHPad.Released.Green);
		PA_OutputText(1, 3,  8, "Red    : %d-%d-%d", GHPad.Newpress.Red,    GHPad.Held.Red,    GHPad.Released.Red);
		PA_OutputText(1, 3,  9, "Yellow : %d-%d-%d", GHPad.Newpress.Yellow, GHPad.Held.Yellow, GHPad.Released.Yellow);		
		PA_OutputText(1, 3, 10, "Blue   : %d-%d-%d", GHPad.Newpress.Blue,   GHPad.Held.Blue,   GHPad.Released.Blue);	
		PA_OutputText(1, 3, 12, "Anykey : %d-%d-%d", GHPad.Newpress.Anykey, GHPad.Held.Anykey, GHPad.Released.Anykey);	

		// For each key, we'll show DblClick, Uptime and Downtime
		PA_OutputText(0, 3,  7, "Green  : %d-%d-%d         ", GHPad.DblClick.Green,  GHPad.Uptime.Green,  GHPad.Downtime.Green);
		PA_OutputText(0, 3,  8, "Red    : %d-%d-%d         ", GHPad.DblClick.Red,    GHPad.Uptime.Red,    GHPad.Downtime.Red);
		PA_OutputText(0, 3,  9, "Yellow : %d-%d-%d         ", GHPad.DblClick.Yellow, GHPad.Uptime.Yellow, GHPad.Downtime.Yellow);		
		PA_OutputText(0, 3, 10, "Blue   : %d-%d-%d         ", GHPad.DblClick.Blue,   GHPad.Uptime.Blue,   GHPad.Downtime.Blue);	
		PA_OutputText(0, 3, 12, "Anykey : %d-%d-%d         ", GHPad.DblClick.Anykey, GHPad.Uptime.Anykey, GHPad.Downtime.Anykey);	

		if (PA_DetectGHPad()) PA_OutputText(1, 3, 17, "Grip is inserted");	
		else PA_OutputText(1, 3, 17, "                ");	
		

		PA_WaitForVBL(); // PA_WaitForVBL automatically updates GHPad values
	}
	
	return 0;
} // End of main()
