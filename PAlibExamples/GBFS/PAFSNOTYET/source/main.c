/* 

*/


// Includes
#include <PA9.h>       // Include for PA_Lib

typedef struct{
	char Name[20];
	char Ext[6];
	u32 Length;
	u32 FilePos;
}PAFSFiles;

typedef struct{
	char StartText[6]; // Début...
	u16 Nfiles; // Nombre de fichiers
} PAFSSystems;

PAFSFiles *File; // Pointeur sur les infos fichiers...

// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	PA_InitText(0, 0);  // Initialise the text system on the bottom screen	
	PA_InitText(1, 0);  // Initialise the text system on the top screen
	
	PA_OutputText(1, 0, 0, "Loading PAFS...");
	
	char *PAFSStart =  (char*)0x08000000;
	//PAFSStart+=256-7;
	
	while (!((PAFSStart[0] == 'P') && (PAFSStart[1] == 'A') && (PAFSStart[2] == 'F') && (PAFSStart[3] == 'S') && (PAFSStart[4] == '0'))) PAFSStart+=1;
	
	//PAFSStart+= 18;
	PAFSSystems *PAFSSys = (PAFSSystems*)PAFSStart;
	PAFSFiles *File = (PAFSFiles*)(((u8*)PAFSStart)+6);
	//PAFSStart += 6; // On passe le texte...
	

	s32 i;
	
	//PAFSFiles *PAFSFile = (PAFSFiles*)PAFSStart;
	
	/*for (i = 0; i < 20; i++) {
		nfiles = *PAFSStart;	PAFSStart += 1;
		PA_OutputText(1, 0, 4+i, "%d    ", nfiles);
	}*/
	
	PA_OutputText(1, 0, 1, "%d    ", ((u32)PAFSStart)-0x08000000);
	PA_OutputText(1, 0, 2, "%d  ", ((u32)PAFSStart)&255);	
	//PA_OutputText(1, 0, 3, "%d    ", nfiles);
	PA_OutputText(1, 0, 3, "Files : %d    ", PAFSSys->Nfiles);		
	
	char *letter = PAFSStart;
	u16 j = 0;
	for (i = 0; i < 4; i++){
		
		for (j = 0; j < 32; j++)  {
			PA_SetTileLetter(0, j, i, *letter);   
			letter++;
//		PA_WaitForVBL();
		}
	}
	for (i = 0; i < 4; i++){
		PA_OutputText(1, 0, 4+i, File[i].Name);		
	}
	
	
	
	// Infinite loop to keep the program running
	while (1)
	{ 
			
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
