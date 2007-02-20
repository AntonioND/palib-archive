/* The text is drawn on a background (you can chose which background you want, 
and can display information on the screen
*/


// Includes
#include <PA9.h>       // Include for PA_Lib

#include "font/all_gfx.h"
#include "font/all_gfx.c"


// Define the text borders
#define X1 0   // From left to right 
#define X2 31
#define Y1 24  // Under the screen !
#define Y2 31


void PA_InitTextBorders(bool screen, u8 x1, u8 y1, u8 x2, u8 y2);


// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	// Load a custom text font
	PA_InitCustomText(1, //screen
					  0, //background number
					  newfont); //font name

	PA_InitTextBorders(1, X1, Y1, X2, Y2);  // Inits the text border !
	
	s16 i;
	s16 letter = 0; s16 nletters = 0;
	
	
	
	for(i = 0; i < 65; i++) {// We'll make the textbox enter slowly in...
		PA_BGScrollY(1, 0, i);
		PA_WaitForVBL();
	}

		
	while (letter == nletters){ // Do this until the function outputs all the text
		++nletters; // Next frame, one more letter...	
		// This text function outputs a given number of letters... This way, it can do as if you were typing !
		letter = PA_SimpleBoxText(1, "Hi there :p  Cool, the function works perfectly ! So you see the text being typed...\n Line break works...", nletters); //%cX changes to color number X
		PA_WaitForVBL(); // You can set more WaitForVBL if you want to slow down the text output...
	}
	
	// Wait a second
	for(i = 0; i < 60; i++) PA_WaitForVBL();
	
	for(i = 64; i >= 0; i--) {// We'll make the textbox move out
		PA_BGScrollY(1, 0, i);
		PA_WaitForVBL();
	}
	
	PA_EraseTextBox(1); // Erases the textbox on screen 1
	
	// Wait a second
	for(i = 0; i < 60; i++) PA_WaitForVBL();
	
	for(i = 0; i < 65; i++) {// We'll make the textbox enter slowly in...
		PA_BGScrollY(1, 0, i);
		PA_WaitForVBL();
	}
		
	letter = 0; nletters = 0; // reinit varaibles
	while (letter == nletters){ // Do this until the function outputs all the text
		++nletters; // Next frame, one more letter...	
		// This text function outputs a given number of letters... This way, it can do as if you were typing !
		letter = PA_SimpleBoxText(1, "And it comes back in with another text !", nletters); //%cX changes to color number X
		PA_WaitForVBL(); // You can set more WaitForVBL if you want to slow down the text output...
	}
	
	// Wait a second
	for(i = 0; i < 60; i++) PA_WaitForVBL();
	
	for(i = 64; i >= 0; i--) {// We'll make the textbox move out
		PA_BGScrollY(1, 0, i);
		PA_WaitForVBL();
	}	
	
	

	// Infinite loop to keep the program running
	while (1)
	{
		
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()






