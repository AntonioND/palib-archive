/* Text in different colors...
*/


// Includes
#include <PA9.h>       // Include for PA_Lib


char text[200]; // This will be our text...



// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	// Initialise the text system on the top screen
	PA_InitText(0, // Bootom screen
				1);  	// Background number, from 0-3, on which to load the text system
	PA_InitText(1, 0);  // Initialise the text system on the top screen

	u8 i;
	for (i = 0; i < 8; i++)
	{
		PA_SetTextTileCol(1, i); // Change the color on the top screen, values 0 to 6
		PA_OutputSimpleText(1, 2, i, "Color test...");  // Screen 1 has different colors
		PA_OutputSimpleText(0, 2, i, "No Color test...");  // Screen 0 was not modified...
	}
	
	//PA_LoadTiledBg(0, 0, PA_text2);

	
	// Infinite loop to keep the program running
	while (1)
	{
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
