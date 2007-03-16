/* FAT functions....
 * Play Sound
 * saberoff.raw must be stored in the root of your cart/card
*/

// Includes
#include <PA9.h>       // Include for PA_Lib
#include <fat.h>
//#include <stdio.h>


// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	PA_InitSound(); //Init sound
	
	PA_InitText(1, 0);  // Initialise the text system on the top screen
	
	PA_WaitForVBL();  PA_WaitForVBL();  PA_WaitForVBL();  // wait a few VBLs
	
	fatInitDefault(); //Initialise fat library

	PA_OutputSimpleText(1, 1, 1, "libfat - play raw");
	PA_OutputSimpleText(1, 1, 3, "Open saberoff.raw for read");	
	FILE* rawFile = fopen ("saberoff.raw", "rb"); //rb = read
	PA_OutputSimpleText(1, 1, 5, "Determine file size");

	// obtain file size:
	u32 rawSize;
   fseek (rawFile , 0 , SEEK_END);
   rawSize = ftell (rawFile);
   rewind (rawFile);

   // allocate memory to store the raw file
   char * buffer;
   buffer = (char*) malloc (sizeof(char)*rawSize);
	
	PA_OutputSimpleText(1, 1, 8, "Copying sound to buffer");   
   fread (buffer,1,rawSize,rawFile);
   // close the file
   fclose (rawFile);
	PA_OutputSimpleText(1, 1, 9, "Complete File Closed");   
	
	//plau the sound	
	PA_PlaySound(0, buffer, rawSize, 127, 11025);

	PA_OutputSimpleText(1, 1, 12, "Press A to play again");   

	while (1)
	{ 
		if (Pad.Newpress.A) 
		{ 
		   PA_StopSound(0); 		
		  	PA_PlaySound(0, buffer, rawSize,127, 11025);
		}  	
	
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()












