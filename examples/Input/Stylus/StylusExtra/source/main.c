/* PAlib has a couple handy variables to detect special
   stylus movements, here we will learn how to read them!
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
		if (Stylus.Held)     PA_OutputSimpleText(1, 0, 7,  "The stylus is on the screen");
		if (Stylus.Newpress) PA_OutputSimpleText(1, 0, 8,  "The stylus has just been pressed");
		if (Stylus.Released) PA_OutputSimpleText(1, 0, 9,  "The stylus has been released");
		if (Stylus.DblClick) PA_OutputSimpleText(1, 0, 10, "The stylus has been double-       clicked");

		PA_OutputText(1, 1, 12, "Stylus held down for %d seconds   ", Stylus.Downtime/60); // Stylus.Downtime/Uptime in VBL time, 60/second
		PA_OutputText(1, 1, 13, "Stylus kept up for   %d seconds   ", Stylus.Uptime/60);		

		// Get the stylus position and show it on screen
		PA_OutputText(1, 1, 15, "Stylus Position: %d, %d   ", Stylus.X, Stylus.Y);

		PA_WaitForVBL();
	}
}