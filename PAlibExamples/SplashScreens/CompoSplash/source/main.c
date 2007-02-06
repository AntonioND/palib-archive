// Include any GIF file and output it on an 8bit or 16bit screen !

// Includes
#include <PA9.h>       // Include for PA_Lib

#include "compo_top.h" // gif to include
#include "compo_bottom.h" // gif to include

// Function: main()
int main(int argc, char ** argv){

	PA_Init();    // PA Init...
	PA_InitVBL();	// VBL Init...
	
	PA_Init8bitBg(0, 3); 	// Init a 16 bit Bg on screen 0
	PA_Init8bitBg(1, 3); 	// Init a 8 bit Bg on screen 1	
	
	PA_SetBrightness(0, -31); // all black
	PA_SetBrightness(1, -31); // all black	
	
	// Load splash while hidden
	PA_LoadGif(	1, (void*)compo_top); // Gif File
	PA_LoadGif(	0, (void*)compo_bottom); // Gif File

	s8 i;
	// Transition to normal visible background
	for(i = -31; i <= 0; i++){
		PA_SetBrightness(0, i); 
		PA_SetBrightness(1, i); 
		PA_WaitForVBL();		   
	}   

	s16 time = 180; // 180 frames = 3 seconds
	while(time && (!Pad.Newpress.Anykey) && (!Stylus.Newpress)){ // Display until time over or keys pressed
		time--; // time goes by
		PA_WaitForVBL();
	}		
	
	// Transition to black
	for(i = 0; i > -32; i--){
		PA_SetBrightness(0, i); 
		PA_SetBrightness(1, i); 
		PA_WaitForVBL();		   
	}  	

	// Now that it's all black, clean all that and you're ready to go !
	PA_ResetBgSys();
	PA_SetBrightness(0, 0); // normal
	PA_SetBrightness(1, 0); // normal		

  while(1)  {
		PA_WaitForVBL();
	}
		
	return 0;
} // End of main()




