// Load any image included with PAFS without even caring about the image format !

// Includes
#include <PA9.h>       // Include for PA_Lib


// Function: main()
int main(int argc, char ** argv){

	PA_Init();    // PA Init...
	PA_InitVBL();	// VBL Init...
	
	PA_Init16bitBg(0, 3); 	// Init a 16 bit Bg on screen 0
	PA_Init16bitBg(1, 3); 	// Init a 16 bit Bg on screen 1	
	
	PA_FSInit(); // Inits PA File System, and returns the number of files
		
		
	PA_LoadFSImage(1, // Screen
						0); // First file

	PA_LoadFSImage(0, // Screen
						1); // Second file

  while(1)  {
		PA_WaitForVBL();
	}
		
	return 0;
} // End of main()




