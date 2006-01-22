/* This one shows how the folders are organized !
*/

// Includes
#include <PA9.h>       // Include for PA_Lib

u16 foldery = 6; // Where to write the folder names
u16 indent = 0; // Indents...

void WriteFolders(u16 N){ // Recursive function to write the number of folders

u16 Nfolder = N;	
	// Write the folder's name...
PA_OutputText(1, indent, foldery, "%s : %d folders, %d files", PA_FSFolder[Nfolder].Name, PA_FSFolder[Nfolder].NFolders, PA_FSFolder[Nfolder].NFiles); 
	foldery++;	
	
	if (PA_FSFolder[Nfolder].NFolders > 0){ // For every subfolder, check their subfolders
		indent+=2; // Indents for the subfolders	
		s32 k;		
		for (k = 0; k <  PA_FSFolder[Nfolder].NFolders; k++){  
			// Note : PA_FSFolder[i].FirstFile stores the number of the first file, 
			//so the files go from 
			//PA_FSFolder[i].FirstFile to PA_FSFolder[i].FirstFile + PA_FSFolder[i].NFiles
			WriteFolders(PA_FSFolder[Nfolder].FirstFolder + k); // Check the subfolders
		}
		indent -= 2;
	}
}


// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	PA_InitText(0, 0);  // Initialise the text system on the bottom screen	
	PA_InitText(1, 0);  // Initialise the text system on the top screen
	
	PA_OutputText(1, 0, 0, "Loading PAFS...");
	

	u32 FileNumer = PA_FSInit(); // Inits PA File System, and returns the number of files

	PA_OutputText(1, 0, 1, "  Files   : %d    ", FileNumer);		
	PA_OutputText(1, 0, 2, "  Folders : %d    ", PA_FSSys->Nfolders);	
			
	// For each file, we'll draw it's name on the bottom screen...

	WriteFolders(0); // Start with the first folder
	
	s32 i;	// Write all the files on the bottom screen
	for (i = 0; i < FileNumer; i++){
		PA_OutputText(0, 0, i+1, "%s.%s   %d kB", PA_FSFile[i].Name, PA_FSFile[i].Ext, PA_FSFile[i].Length);
	}

	// Infinite loop to keep the program running
	while (1)
	{ 

			
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
