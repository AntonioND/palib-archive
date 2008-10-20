// Include any GIF file with PAFS, and output it on a 16bit screen !

// Includes
#include <PA9.h>       // Include for PA_Lib


// Function: main()
int main(int argc, char ** argv){

	PA_Init();    // PA Init...
	PA_InitVBL();	// VBL Init...
	
	PA_Init16bitBg(0, 3); 	// Init a 16 bit Bg on screen 0
	PA_Init8bitBg(1, 3); 	// Init a 16 bit Bg on screen 1	
	
	PA_FSInit(); // Inits PA File System, and returns the number of files
		
	PA_LoadGif(	1, // Screen, which is 8 bit...
				PA_PAFSFile(0)); // Gif File
	PA_LoadGif(	0, // Screen, which is 16 bit
				PA_PAFSFile(1)); // Gif File


  while(1)  {
		PA_WaitForVBL();
	}
		
	return 0;
} // End of main()




