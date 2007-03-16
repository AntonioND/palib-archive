// Sound example ! Add the sound raw file 

// Includes
#include <PA9.h>       // Include for PA_Lib
#include "saberoff.h"  // Include the sound (found in the data folder in .raw format)


// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	PA_InitText(0, 0);
	
	
	PA_InitSound();  // Init the sound system
	
	PA_PlaySoundRepeat(0, saberoff);

	// Infinite loop to keep the program running
	while (1)
	{	

		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
