/* This shows how to use the basic PAFS functions ! PAFS is a file system for nintendo DS, which supports multiple files in multiple directories... See the PAFSFolders to see how to detect the different folders

*/

// Includes
#include <PA9.h>       // Include for PA_Lib

char filename[20]; // Temporary string for file names

// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	PA_InitText(0, 0);  // Initialise the text system on the bottom screen	
	
	PA_InitText(1, 0);  // Initialise the text system on the top screen
	
	PA_WaitForVBL();  PA_WaitForVBL();  PA_WaitForVBL();  // wait a few VBLs
	
	FAT_InitFiles();  // FAT driver init

	u8 filetype = FAT_FindFirstFile(filename);
	if(filetype == 1) PA_OutputText(1, 0, 1, "First File : %s    ", filename);	  // 1 if file
	else if(filetype == 2) PA_OutputText(1, 0, 1, "First Folder : %s    ", filename);	// 2 if folder
	else 	PA_OutputText(1, 0, 1, "No File found"); // no file or folder...
	
	u8 i;
	// Find a few files...
	for (i = 1; (i < 20)&&(filetype!= 0); i++){  // search for 10 files... stops if no file found
		filetype = FAT_FindNextFile(filename);
		if(filetype == 1) PA_OutputText(1, 0, i, "File : %s    ", filename);	  // 1 if file
		else if(filetype == 2) PA_OutputText(1, 0, i, "Folder : %s    ", filename);	// 2 if folder
		else 	PA_OutputText(1, 0, i, "Nothing else"); // no file or folder...
	}	


	while (1)
	{ 
			
			
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()












