// Include any GIF file with GBFS, and output it on a 16bit screen !

// Includes
#include <PA9.h>       // Include for PA_Lib

#include "i1sec.h" // gif to include


// Function: main()
int main(int argc, char ** argv){

	PA_Init();    // PA Init...
	PA_InitVBL();	// VBL Init...
	
	PA_Init8bitBg(1, 3); 	// Init a 8 bit Bg on screen 1	
	PA_InitText(0, 0);
	
	PA_OutputSimpleText(0, 2, 4, "First gif at 1 frame/second");
	PA_SetGifSpeed(60); // Number of VBLS to wait between 2 images. 60 for 1 second...
	PA_LoadGif(1, (void*)i1sec);  // Playing the animation at a speed of 1 frame/second
	
	PA_OutputSimpleText(0, 2, 4, "Second gif at 6 frames/second");	
	PA_SetGifSpeed(10); // This time, let's do it way faster...
	PA_LoadGif(1, (void*)i1sec); 


  while(1)  {
		PA_WaitForVBL();
	}
		
	return 0;
} // End of main()




