// Includes 
#include <PA9.h>       // Include for PA_Lib

#include "gfx/all_gfx.h"
#include "gfx/all_gfx.c"


char text[2000] = {};

s16 nletter = 0; // no text yet...
s16 maxletters = 0; // numbers of letter in the text...
char letter;   // letter given by the keyboard presses...
s8 textsize = 2;    // text size....


void KeyBoardType(void); // Function that types the text nicely...



// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	

	PA_Init16cBg(0, 3);  // 16 color background init with default colors for text	
	PA_Init16cBg(1, 3); 
	
	
	PA_16cText(0, 10, 10, 245, 181, "Press Up/Down to change font size\nPress Left/Right to move cursor", 1, 3, 2048);
	
		
	PA_InitKeyboard(2); // Load the keyboard on background 2...screen 0...
	
	PA_KeyboardIn(20, 100);  // Make it come in
	
	PA_InitText(0, 0);
	

	PA_LoadSpritePal(1, 0, (void*)Thingy_Pal);  
	PA_CreateSprite(1, 0, (void*)Thingy_Sprite, OBJ_SIZE_8X16, 1, 0, 0, 0);  // Cursor that shows where you are typing...
	
	// Infinite loop to keep the program running
	while (1)
	{
		
		KeyBoardType(); // Use the keyboard if on bottom screen...
		
		

		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()





void KeyBoardType(void){
s16 i;
		// Change text size... 
		u8 oldtextsize = textsize;
		textsize += Pad.Newpress.Up - Pad.Newpress.Down; // Change text size
		if (textsize < 0) textsize = 0;
		else if (textsize > 4) textsize = 4; // Limit textsize range to 0-4
		
		// Adjust cursor position according to keys...
		nletter += Pad.Newpress.Right - Pad.Newpress.Left;
		if (nletter > maxletters) nletter--;
		if (nletter < 0) nletter = 0;		
		
		letter = PA_CheckKeyboard();  // Check PA Keyboard
		
		if ((letter)||(oldtextsize != textsize)){  // If new press or size changed...
			if ((letter > 31)||(letter == '\n')) { // there is a new letter
				for(i = maxletters; i >= nletter; i--) text[i+1] = text[i];  // Save following letters
				text[nletter] = letter;
				nletter++;
			}
			else if ((letter == PA_BACKSPACE)&&nletter) { // Backspace pressed
				for(i = nletter-1; i <maxletters+1; i++) text[i] = text[i+1];  // copy all following letters...
				nletter--;
			}


			PA_16cErase(1); // erase before drawing text...
			maxletters = PA_16cText(1, 1, 1, 255, 191, text, 1, textsize, 2048);	// Display the text transformed :)	
		}
		if (maxletters) PA_SetSpriteXY(1, 0, PA_16cLetterPos.Letter[nletter].X-1, PA_16cLetterPos.Letter[nletter].Y); // position of letter...
}	






