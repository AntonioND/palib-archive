// Drawing on DS is easy... This'll show you to what point !

// Includes
#include <PA9.h>       // Include for PA_Lib




// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL

	PA_Init16bitDblBuffer(0, 3); // Double buffer... Draw on the hidden background, then show
	PA_InitText(1, 0); // Text background
	
	PA_OutputText(1, 4, 10, "Press A to swap buffer...");
		

	// Infinite loop to keep the program running
	while (1)
	{
		if(Pad.Newpress.A) PA_16bitSwapBuffer(0); // Swap buffer...
		
		// Simple draw function, draws on the screen...
		PA_16bitDraw(0, // Screen
					PA_RGB(31, 0, 0));  // Palette color
					

		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
