/* This example will show how to easy GBFS is actually is to use !
GBFS is a file system for gba and DS. What for ? Well, you can add files after compilation !

We'll include a short text file using GBFS, and output it on the screen...

In the folder, you must have padbin, gbfs.exe, GBFSinclude.bat, and the files you want to include.
The GBFSinclude bat file will include all .txt files to the rom, you can edit it to add other types
The outputed file is GBFS2.ds.gba (GBFS being the one without )

Here's the explaination of the GBFSinclude.bat :
gbfs.exe temp.gbfs *.txt    -> create a gbfs file from all the txt files. 
padbin 256 GBFS.nds			-> round up the rom to 256 bytes
cat GBFS.nds temp.gbfs >boot.nds	-> put the original NDS rom file and the GBFS file into a single one
del temp.gbfs						-> delete the GBFS file, not needed anymore
cat ndsmall.bin boot.nds >GBFS2.ds.gba	-> create a GBFS2.ds.gba file from the .NDS...
pause				-> That's it !

!!!! Don't forget to run GBFSinclude.bat !!!!

*/


// Includes
#include <PA9.h>       // Include for PA_Lib



// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	PA_InitText(0, 0);  // Initialise the text system on the bottom screen	
	PA_InitText(1, 0);  // Initialise the text system on the top screen

	u8 nfiles = PA_InitGBFS(); // Init GBFS and get the number of files...
	PA_OutputText(1, 0, 0, "Files : %d", nfiles); // Show the number of files we added

	u8 i;
	// Now, we'll output all the files names, extensions, and sizes...
	for (i = 0; (i < nfiles)&&(i < 22); i++)
		PA_OutputText(1, 0, i+2, "%s.%s %d  ", PA_GBFSfile[i].Name, PA_GBFSfile[i].Ext, PA_GBFSfile[i].Length);
		// respectively : file name, file extension, file length

	// Let's search for a file named hi...
	s8 hifile = PA_GetGBFSFile(0, // start from file 0
							   "hi", // search for a 'hi' file
								""); // No particular extension
	if (hifile > -1) {// the PA_GetGBFSFile 
		PA_OutputText(1, 20, 0, "hi.txt : %d", hifile);
	}


	if (nfiles) { // If there is at least a file
		// And the proof of concept will be display the first file's text on the bottom screen...
		PA_BoxText(0, 0, 0, 31, 23, PA_GBFSfile[0].File, PA_GBFSfile[0].Length); 
	}
	
	s8 nfile = 0; // File to show...
	
	// Infinite loop to keep the program running
	while (1)
	{ 
		if (Pad.Newpress.Down) {
			nfile++; // Use the next file !
			if (nfile == nfiles) nfile--; // Never go beyond the maximum number of files
			PA_OutputSimpleText(0, 0, 0, "                                                        ");
			PA_BoxText(0, 0, 0, 31, 23, PA_GBFSfile[nfile].File, PA_GBFSfile[nfile].Length); // Show it
		}
		if (Pad.Newpress.Up) { // Same thing...
			nfile--; // Use the next file !
			if (nfile < 0) nfile++; // Never go beyond the maximum number of files
			PA_OutputSimpleText(0, 0, 0, "                                                        ");
			PA_BoxText(0, 0, 0, 31, 23, PA_GBFSfile[nfile].File, PA_GBFSfile[nfile].Length); // Show it
		}		
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
