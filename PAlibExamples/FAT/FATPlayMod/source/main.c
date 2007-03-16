/* FAT functions....
 * Play Mod file.  
 * modfile.mod must be stored in the root of your cart/card
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

	PA_OutputSimpleText(1, 1, 1, "libfat - play mod");
	PA_OutputSimpleText(1, 1, 3, "Open modfile.mod for read");	
	FILE* modFile = fopen ("modfile.mod", "rb"); //rb = read
	PA_OutputSimpleText(1, 1, 5, "Determine mod size");

	// obtain file size:
	u32 modSize;
   fseek (modFile , 0 , SEEK_END);
   modSize = ftell (modFile);
   rewind (modFile);
	PA_OutputText(1, 1, 6, "Size:%d", modFile);	

   // allocate memory to store the mod file
   char * buffer;
   buffer = (char*) malloc (sizeof(char)*modSize);
	
	PA_OutputSimpleText(1, 1, 8, "Copying mod to buffer");   
   fread (buffer,1,modSize,modFile);
   // close the file
   fclose (modFile);
	PA_OutputSimpleText(1, 1, 9, "Complete File Closed");   
	
	PA_OutputSimpleText(1, 1, 12, "Play mod");   
	PA_PlayMod(buffer);
		
	
	//nothing to do but wait....
	while (1)
	{ 
			
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()












