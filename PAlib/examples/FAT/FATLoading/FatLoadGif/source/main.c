/* FAT functions....
 * Laod GIF file.  
 * YourGIFFile.gif must be stored in the root of your cart/card
*/

// Includes
#include <PA9.h>       // Include for PA_Lib
#include <fat.h>


// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	PA_Init8bitBg(1, 3); 	// Init a 8 bit Bg on screen 1	
	
	PA_WaitForVBL();  PA_WaitForVBL();  PA_WaitForVBL();  // wait a few VBLs
	
	fatInitDefault(); //Initialise fat library


	FILE* imgFile = fopen ("YourGIFFile.gif", "rb"); //rb = read
	// If you want to use folders them make sure you add them like this.
        // Be aware that some card are case-sensitive.
	// FILE* imgFile = fopen ("/MyFolder/YourGIFFile.gif", "rb");
	
	if(imgFile){ // Make sure there is a file to load
		// obtain file size:
		u32 imgSize;
		fseek (imgFile, 0 , SEEK_END);
		imgSize = ftell (imgFile);
		rewind (imgFile);
	
		// allocate memory to store the image file
		char * buffer;
		buffer = (char*) malloc (sizeof(char)*imgSize);
		fread (buffer, 1, imgSize, imgFile);
	
		// close the file
		fclose (imgFile);
		PA_LoadGif(1, (void *)buffer); // Show GIF on top screen
	}
		
	
	//nothing to do but wait....
	while (1)
	{ 
			
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()

