////////////////////////////
// PAlib project template //
////////////////////////////

// Lines starting with two slashes are ignored by the compiler
// Basically you can use them to comment what are you doing
// In fact, this kind of lines are called comments :P

// Include PAlib so that you can use it
#include <PA9.h>

int main(){
	// Initialize PAlib
	PA_Init();

	// Put your initialization code here

	// Infinite loop to keep the program running
	while(true){
		// Put your game logic here

		// Wait until the next frame.
		// The DS runs at 60 frames per second.
		PA_WaitForVBL();
	}
}
