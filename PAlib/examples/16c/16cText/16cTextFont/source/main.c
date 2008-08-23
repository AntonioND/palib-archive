// Includes
#include <PA9.h>       // Include for PA_Lib

// Include de newly converted fonts...
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"



// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	

	
	PA_Init16cBg(0, 3); 	PA_Init16cBg(1, 3);  // 16 color background init with default colors for text

	// Slots 0-4 are already used by default PAlib fonts, so we'll load on slots 5 and 6. 
	// Available slots : 5-9. You can overwrite a PAlib font though if you want to use slot 0 for example...
	PA_16cCustomFont(5, smallfont);
	PA_16cCustomFont(6, bigfont);	



	PA_16cText(1, 10, 25, 255, 40, "Small custom font...", 1, 5, 100);
	PA_16cText(1, 10, 40, 255, 60, "Big custom font...", 1, 6, 100);
	
	u8 i;

	// Different colors on the bottom sreen...
	for (i = 0; i < 10; i++){
		PA_16cText(0, 10, i*20, 255, i*20 + 20, "Hello World", i+1, 6, 100);
	}		
	
	// Infinite loop to keep the program running
	while (1)
	{
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
