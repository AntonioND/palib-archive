/* This shows how to use the basic PAFS functions ! PAFS is a file system for nintendo DS, which supports multiple files in multiple directories... The Ram version works in emulators and WMB !!

*/

// Includes
#include <PA9.h>       // Include for PA_Lib


PA_FSRam(100000);  // This defines the size of the memory you allocate for files at the maximum ... I set it to 100kb, here. You can use more (like 2 megs, but not much more) or as little as a few KB if you don't need to put much in it...


// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	PA_InitText(0, 0);  // Initialise the text system on the bottom screen	
	PA_InitText(1, 0);  // Initialise the text system on the top screen
	
	PA_OutputText(1, 0, 0, "Loading PAFS...");

	PAFSStart = (char*)PA_FileSystem; // Tells to use the ram...
	u32 FileNumber = PA_FSRamInit(); // Inits PA Ram File System, and returns the number of files

	PA_OutputText(1, 0, 1, "  Files   : %d    ", FileNumber);		
	//PA_OutputText(1, 0, 2, "  Folders : %d    ", PA_FSSys->Nfolders);	
			
	// For each file, we'll draw it's name on the bottom screen...
	s32 i;	
	for (i = 0; i < FileNumber; i++){
		PA_OutputText(0, 0, i, "%s.%s   Size : %d, %d", PA_FSFile[i].Name, PA_FSFile[i].Ext, PA_FSFile[i].Length, PA_FSFile[i].FilePos);
	}
	
	// Output the first file text on the top screen, just to test...
	PA_OutputText(1, 0, 5, "File 0 : %s", PA_PAFSFile(0));
	PA_OutputText(1, 0, 7, "File 1 : %s", PA_PAFSFile(1));
	PA_OutputText(1, 0, 9, "File 2 : %s", PA_PAFSFile(2));	
	
	// Infinite loop to keep the program running
	while (1)
	{ 
			
			
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()












