#include <PA9.h>


int main(void){

	PA_Init(); //PAlib inits
	PA_InitVBL();
	
	PA_InitText(0, 0);  // Initialise the text system on the bottom screen	
	
	PA_InitText(1, 0);  // Initialise the text system on the top screen
	
	PA_OutputText(1, 0, 0, "Loading PAFS...");

	u32 FileNumber = PA_FSInit(); // Inits PA File System, and returns the number of files

	PA_OutputText(1, 0, 1, "  Files   : %d    ", FileNumber);		
	
	// Displays the present files :
	s32 i;	
	for (i = 0; i < FileNumber; i++){
		PA_OutputText(0, 0, i, "%d : %s", i, PA_FSFile[i].Name);
	}
		
	PA_LoadSpritePal(0, // Screen
					0, // Palette number
					PA_PAFSFile(0));	// Palette... using the first PAFS file :)
					
	PA_CreateSprite(0, // Screen
					0, // Sprite number
					PA_PAFSFile(1), // Sprite, using the second PAFS file :)
					OBJ_SIZE_32X32, // Sprite size
					1, // 256 color mode
					0, // Sprite palette number
					50, 50); // X and Y position on the screen
				

	while(1) // Infinite loops
	{
	
	}
	
return 0;
}
