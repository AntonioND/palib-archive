// Include any GIF file and output it on an 8bit or 16bit screen !

// Includes
#include <PA9.h>       // Include for PA_Lib

#include "Mollusk.h" // gif to include
#include "trans.h" // gif to include

// Function: main()
int main(int argc, char ** argv){

	PA_Init();    // PA Init...
	PA_InitVBL();	// VBL Init...
	
	PA_Init16bitBg(0, 3); 	// Init a 16 bit Bg on screen 0
	PA_Init8bitBg(1, 3); 	// Init a 8 bit Bg on screen 1	
	
	PA_LoadGif(	1, // Screen, which is 8 bit...
				(void*)Mollusk); // Gif File

	PA_LoadGif(	0, // Screen, which is 16 bit
				(void*)trans); // Gif File



  while(1)  {
		PA_WaitForVBL();
	}
		
	return 0;
} // End of main()




