/* Here you'll see all the stylus information, which is really not much :
- Stylus location
- Stylus newly pressed, held, or released...
*/


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
		
	PA_WaitForVBL();

	u8 nletter = 0;
	
	// Infinite loop to keep the program running
	while (1)
	{
	   
	   if(Stylus.Newpress) PA_Clear8bitBg(0); // Reset the screen when we start a new character
	   
	   PA_8bitDraw(0, 1);
	   	   
		char letter = PA_CheckLetter(); // Returns the letter !!!
		
		if (letter > 31) { // there is a new letter
			text[nletter] = letter;
			nletter++;
		}
		else if ((letter == PA_BACKSPACE)&&nletter) { // Backspace pressed
			nletter--;
			text[nletter] = ' '; // Erase the last letter
		}
		else if (letter == '\n'){ // Enter pressed
			text[nletter] = letter;
			nletter++;
		}
		
		PA_OutputText(1, 2, 2, text); // Write the text
		
		PA_OutputSimpleText(1, 0, 10, "Draw a PAGraffiti letter to have it recognized by the system...");
		
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
