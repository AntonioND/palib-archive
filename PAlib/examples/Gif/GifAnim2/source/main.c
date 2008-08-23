// Advanced gif animation example

// Includes
#include <PA9.h>       // Include for PA_Lib

#include "bartman.h" // gif to include


	float speed = 1; // gif speed, normal speed by default...
	u8 pause = 0; // pause/unpause
	
	s32 StartFrame = 0; // frame from which to start
	s32 EndFrame = 20; // frame from which to end




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
	
	PA_OutputText(0, 10, 11, "Speed : %f2     ", speed);	

// Change starting frame...
	if (Pad.Newpress.B) StartFrame += 1; 
	if (Pad.Newpress.A && (StartFrame > 0)) StartFrame -= 1;
	PA_OutputText(0, 2, 13, "Start Frame : %d     ", StartFrame);	
	PA_GifSetStartFrame(StartFrame); // Set the starting frame number...

// Change ending frame...
	if (Pad.Newpress.Y) EndFrame += 1; 
	if (Pad.Newpress.X && (EndFrame > 0)) EndFrame -= 1;
	PA_OutputText(0, 2, 14, "End Frame  : %d     ", EndFrame);	
		PA_GifSetEndFrame(EndFrame); // Set the ending frame number...

	PA_OutputText(0, 4, 16, "Current Frame  : %d     ", PA_GifGetFrame());	

}




// Function: main()
int main(int argc, char ** argv){

	PA_Init();    // PA Init...
	PA_InitVBL();	// VBL Init...
	
	PA_Init8bitBg(1, 3); 	// Init a 8 bit Bg on screen 1	
	
	
	PA_InitText(0, 0); // Text on bottom screen
		
	PA_OutputSimpleText(0, 1, 6, "Press Start to pause/unpause");
	PA_OutputSimpleText(0, 1, 7, "Press Up/Down to change speed");
	PA_OutputSimpleText(0, 1, 8, "Press A/B to change StartFrame");
	PA_OutputSimpleText(0, 1, 9, "Press X/Y to change EndFrame");	
	PA_VBLFunctionInit(MyCustomVBL); // start the function to play during gif file !

  while(1)  {
 		PA_LoadGif(	1, (void*)bartman); // Gif File to play over and over again
 		PA_Clear8bitBg(1); // Clear the screen after use
		PA_WaitForVBL();
	}
		
	return 0;
} // End of main()




