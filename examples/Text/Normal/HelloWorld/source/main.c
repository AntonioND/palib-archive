/////////////////////////
// Hello World Program //
/////////////////////////

// Lines starting with two slashes are ignored by the compiler
// Basically you can use them to comment what are you doing
// In fact, this kind of lines are called comments :P

// Include PAlib so that you can use it
#include <PA9.h>

int main(){
	// Initialize PAlib
	PA_Init();

	// Load the default text font
	PA_LoadDefaultText(1, // Top screen
	                   2); // Background #2

	// Write the text "Hello World"
	PA_OutputSimpleText(1, // Top screen
	                    1, // X position 1*8 = 8
	                    1, // Y position 1*8 = 8
	                    "Hello World");

	// Infinite loop to keep the program running
	while(true){
		// Wait until the next frame.
		// The DS runs at 60 frames per second.
		PA_WaitForVBL();
	}
}