// Gif animation example

// Includes
#include <PA9.h>       // Include for PA_Lib

#include "bartman.h" // gif to include


	float speed = 1; // gif speed, normal speed by default...
	u8 pause = 0; // pause/unpause


void MyCustomVBL(void) { // VBL function to play while gif is on...

	if(Pad.Newpress.Start){
		pause = !pause; // if 1 returns 0, if 0 returns 1
		if (pause == 1) {
			PA_GifAnimPause();  // pause animation
			PA_OutputSimpleText(0, 10, 11, "PAUSE !");
		}
		else {
			PA_GifAnimPlay();   // unpause...
			PA_OutputSimpleText(0, 10, 11, "       ");
		}
	}
	
	// Change speed according to keys
	if (Pad.Held.Up) speed += 0.02; 
	if (Pad.Held.Down && (speed > 0.02)) speed -= 0.02; 
	
	PA_GifAnimSpeed(speed);  // Gif speed command :)
	
	PA_OutputText(0, 10, 12, "Speed : %f2     ", speed);	

}




// Function: main()
int main(int argc, char ** argv){

	PA_Init();    // PA Init...
	PA_InitVBL();	// VBL Init...
	
	PA_Init8bitBg(1, 3); 	// Init a 8 bit Bg on screen 1	
	
	
	PA_InitText(0, 0); // Text on bottom screen
	

	
	PA_OutputSimpleText(0, 2, 8, "Press Start to pause/unpause");
	PA_OutputSimpleText(0, 2, 9, "Press Up/Down to change speed");

	PA_VBLFunctionInit(MyCustomVBL); // start the function to play during gif file !

  while(1)  {
 		PA_LoadGif(	1, (void*)bartman); // Gif File to play over and over again
		PA_WaitForVBL();
	}
		
	return 0;
} // End of main()




