// Drawing on DS is easy... This'll show you to what point !

// Includes
#include <PA9.h>       // Include for PA_Lib




// Function: main()
int main()
{
	PA_Init();    // Initializes PA_Lib
	
	// Let's put some colors... in the background palettes
	PA_SetBgPalCol(0, 1, PA_RGB(31, 31, 31));
	PA_SetBgPalCol(0, 2, PA_RGB(31, 0, 0));	
	PA_SetBgPalCol(0, 3, PA_RGB(0, 0, 31));
	PA_SetBgPalCol(0, 4, PA_RGB(0, 31, 0));
	PA_SetBgPalCol(1, 1, PA_RGB(31, 31, 31));
	PA_SetBgPalCol(1, 2, PA_RGB(31, 0, 0));	
	PA_SetBgPalCol(1, 3, PA_RGB(0, 0, 31));
	PA_SetBgPalCol(1, 4, PA_RGB(0, 31, 0));
	


	PA_Init8bitDblBuffer(0, 3); // Double buffer... Draw on the hidden background, then show
	PA_LoadDefaultText(1, 0); // Text background
	
	PA_OutputText(1, 4, 10, "Press A to swap buffer...");
		

	// Infinite loop to keep the program running
	while (1)
	{
		if(Pad.Newpress.A) PA_8bitSwapBuffer(0); // Swap buffer...
		
		// Simple draw function, draws on the screen...
		PA_8bitDraw(0, // Screen
					1);  // Palette color
					

		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()