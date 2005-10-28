// Include any BMP file with GBFS, and output it on a 16bit screen !

// Includes
#include <PA9.h>       // Include for PA_Lib


// Function: main()
int main(int argc, char ** argv){

	PA_Init();    // PA Init...
	PA_InitVBL();	// VBL Init...
	
	PA_Init16bitBg(0, 3); 	// Init a 16 bit Bg on screen 0
	PA_Init16bitBg(1, 3); 	// Init a 16 bit Bg on screen 0	
	PA_InitText(1, 0);  // Initialise the text system on the top screen

	u8 nfiles = PA_InitGBFS(); // Init GBFS and get the number of files...
	PA_OutputText(1, 0, 15, "Files : %d", nfiles); // Show the number of files we added
	
	// output info on the first file...
	PA_OutputText(1, 0, 16, "%s, %dx%d", PA_GBFSfile[0].Name, PA_GetBmpWidth(PA_GBFSfile[0].File), PA_GetBmpHeight(PA_GBFSfile[0].File)); // Name and size...
	// output info on the second file...
	PA_OutputText(1, 0, 17, "%s, %dx%d", PA_GBFSfile[1].Name, PA_GetBmpWidth(PA_GBFSfile[1].File), PA_GetBmpHeight(PA_GBFSfile[1].File)); // Name and size...
	
	
	// Now we simply load the BMP...
	PA_LoadBmp(	0, // Screen
				PA_GBFSfile[0].File); // File to load
	PA_LoadBmp(	1, // Top Screen
				PA_GBFSfile[1].File); // Other file...
				
	while(1){	// Infinite loop
		PA_WaitForVBL();
	}
		
	return 0;
} // End of main()


