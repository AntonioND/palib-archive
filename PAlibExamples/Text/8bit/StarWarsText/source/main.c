// This example shows how to use 8 bit backgrounds, 
// on which you can draw with the stylus or show text in different sizes, colors, or rotations

// Includes
#include <PA9.h>       // Include for PA_Lib



char starwarstext[20][100] = {
	"Star Wars Text Demo",
	"",
	"Brought to you",
	"by Mollusk,",
	"who got bored one",
	"thursday evening...",
	"",
	"Works pretty well ;)",
	"",
	"But it's completely",
	"USELESS !!!",
	".",
	"..",
	"...",
	"THE END !"
};



// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	// Let's put some colors... in the background palettes
	PA_SetBgPalCol(0, 1, PA_RGB(31, 31, 31));
	PA_SetBgPalCol(1, 1, PA_RGB(31, 31, 31));
	
	// Init an 8bit bg...
	PA_InitBig8bitBg(0, 3); // A big one ^^
	PA_Init8bitBg(1, 3);

	s16 i;
	
	PA_CenterSmartText(1, 10, 60, 245, 100, "Star Wars Text Demo", 1, 4, 0);
	
	// Infinite loop to keep the program running
	while (1)
	{
	
		PA_InitMode7(3); // Init Mode7 style for the star wars effect
	
		// Values that look good...
		PA_Mode7Height(32000);
		PA_Mode7MoveLeftRight(-128);
		PA_Mode7MoveForwardBack(150);
	
		for (i = 0; i < 15; i++)	PA_CenterSmartText(0, 20, 16+i*16, 235, 36+i*16, starwarstext[i], 1, 4, 1); // Draw the whole text
	
		for(i = 0 ; (i < 2048); i++) { // Scroll it star wars style
			if((i%5)==0) PA_Mode7MoveForwardBack(-1);
			PA_WaitForVBL();
		}
	
		PA_WaitFor(Stylus.Newpress); // Finished...
	
		PA_DeInitMode7();	 // Reset Mode7
		PA_Clear8bitBg(0); // Erase screen
		

	
	}
	
	return 0;
} // End of main()
