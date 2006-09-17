
// Includes
#include <PA9.h>       // Include for PA_Lib


char text[1000];


// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	
	PA_InitText(1, 0);  // Initialise the text system on the top screen
	PA_Init8bitBg(0, 0); // We'll draw the characters on the screen...
	
	PA_SetBgPalCol(0, 1, PA_RGB(31, 31, 31)); // Set the writing color to white

	PA_UsePAGraffiti(0); // Do not use the provided Graffiti shapes...
	
	PA_RecoAddShape('a', "AAAAAAAAAAAAAAA"); // Straight right-going line
	PA_RecoAddShape('b', "111111111111111"); // Straight left-going line	
	PA_RecoAddShape('c', "IIIIIIIIIIIIIII"); // Straight up-going line	
	PA_RecoAddShape('d', "999999999999999"); // Straight down-going line			
		
	PA_WaitForVBL();

	
	// Infinite loop to keep the program running
	while (1)
	{
	   
	   if(Stylus.Newpress) PA_Clear8bitBg(0); // Reset the screen when we start a new character
	   
	   PA_8bitDraw(0, 1);
	   	  
			   
		char letter[2]; letter[1] = 0;
		letter[0] = PA_CheckLetter(); // Returns the letter !!!
		
		if (Stylus.Released){ // only show if released
			PA_OutputText(1, 2, 7, "Shape Recognized : %s", letter); // Letter recognized
			PA_OutputText(1, 0, 1, "A for right, B for left, C for up, D for down");
		}		
		
		PA_OutputText(1, 4, 22, "Shape : %s", PA_RecoShape); // Write the shape string
		
		PA_OutputSimpleText(1, 0, 10, "Draw a PAGraffiti letter to have it recognized by the system...");
		
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
