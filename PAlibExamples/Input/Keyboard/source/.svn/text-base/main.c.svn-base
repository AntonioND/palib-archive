// Includes
#include <PA9.h>       // Include for PA_Lib


char text[200]; // This will be our text...



// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	PA_InitText(1, 0);  // Initialise the text system
	
	PA_InitKeyboard(2); // Load the keyboard on background 2...
	
	PA_KeyboardIn(20, 100); // This scrolls the keyboard from the bottom, until it's at the right position
	// PA_KeyboardOut() can be used to scroll the Keyboard out
	// PA_ScrollKeyboardXY(x, y) can be used to set the keyboards position
	
	
	PA_OutputSimpleText(1, 7, 10, "Text : "); 
	
	s32 nletter = 0; // Next letter to right. 0 since no letters are there yet
	char letter = 0; // New letter to write.
	
	// Infinite loop to keep the program running
	while (1)
	{
		// We'll check first for color changes, with A, B, and X
		if (Pad.Newpress.A) PA_SetKeyboardColor(0, 1); // Blue and Red
		if (Pad.Newpress.B) PA_SetKeyboardColor(1, 0); // Red and Blue
		if (Pad.Newpress.X) PA_SetKeyboardColor(2, 1); // Green and Red
		if (Pad.Newpress.Y) PA_SetKeyboardColor(0, 2); // Blue and Green
		
		letter = PA_CheckKeyboard();
		
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
		
		PA_OutputSimpleText(1, 8, 11, text); // Write the text
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
