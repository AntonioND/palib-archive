// Includes
#include <PA9.h>       // Include for PA_Lib




// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	

	// Infinite loop to keep the program running
	while (1)
	{

		if (Pad.Newpress.A) PA_SetLed(1, 0); // Blinking, slow
		if (Pad.Newpress.B) PA_SetLed(1, 1); // Blinking, fast
		if (Pad.Newpress.X) PA_SetLed(0, 0); // Always on
		if (Pad.Newpress.Y) PA_SetLed(0, 1); // Always on

		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
