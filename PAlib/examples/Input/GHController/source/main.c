// A simple demo of using the Guitar Hero Grip with PAlib
// by BassAceGold


// Includes
#include <PA9.h>       // Include for PA_Lib


// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	PA_InitText(1,0);

	PA_OutputSimpleText(1, 0, 0, "GH Button Test");
	PA_OutputSimpleText(1, 0, 1, "by BassAceGold");
	PA_OutputSimpleText(1, 0, 3, "For each key, we'll show\n Newpress, Held, and Released");

	PA_InitGHPad(); // Call this first before using other GH controller functions

	// Infinite loop to keep the program running
	while (1)
	{

		PA_OutputText(1, 3, 7, "Green    : %d-%d-%d", GHPad.Newpress.Green, GHPad.Held.Green, GHPad.Released.Green);
		PA_OutputText(1, 3, 8, "Red   : %d-%d-%d", GHPad.Newpress.Red, GHPad.Held.Red, GHPad.Released.Red);
		PA_OutputText(1, 3, 9, "Yellow   : %d-%d-%d", GHPad.Newpress.Yellow, GHPad.Held.Yellow, GHPad.Released.Yellow);		
		PA_OutputText(1, 3, 10, "Blue  : %d-%d-%d", GHPad.Newpress.Blue, GHPad.Held.Blue, GHPad.Released.Blue);	

		if(GHPad.Held.Anykey) PA_OutputText(1, 3, 15, "anykeyworks");	
		else PA_OutputText(1, 3, 15, "              ");

		if(PA_DetectGHPad()) PA_OutputText(1, 3, 17, "Grip is inserted");	
		else PA_OutputText(1, 3, 17, "                ");	
		

		PA_WaitForVBL(); // PA_WaitForVBL automatically updates GHPad values
	}
	
	return 0;
} // End of main()
