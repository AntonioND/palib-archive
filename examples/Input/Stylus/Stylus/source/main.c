/* Here you'll learn how to read the stylus,
   which is what makes the DS special!
*/

// Includes
#include <PA9.h>

int main(){
	PA_Init();
	PA_LoadDefaultText(1, 0);

	int i;

	// Infinite loop to keep the program running
	while(true){
		// Erase the text on the screen...
		for(i = 0; i < 10; i++) PA_OutputSimpleText(1, 0, 7+i, "                                ");

		// Check the stylus presses
		if (Stylus.Held)     PA_OutputSimpleText(1, 0, 7, "The stylus is on the screen");
		if (Stylus.Newpress) PA_OutputSimpleText(1, 0, 8, "The stylus has just been pressed");
		if (Stylus.Released) PA_OutputSimpleText(1, 0, 9, "The stylus has been released");

		// Get the stylus position and show it on screen
		PA_OutputText(1, 1, 11, "Stylus Position: %d, %d   ", Stylus.X, Stylus.Y);

		PA_WaitForVBL();
	}
}