// PA_GetHTTP using dswifi lib example
//
// Make sure you have selected an ARM7 binary with DSWIFI support in your project Makefile!
//

// Includes
#include <PA9.h>       // Include for PA_Lib
#include <stdio.h>

int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
    PA_InitWifi();
    PA_ConnectWifiWFC();
    
	// Initialise the text system on the top screen
	PA_InitText(0,0);
	PA_InitText(1, 0);

	char *buffer2 = malloc(256*256);

    PA_GetHTTP(buffer2, "http://www.google.be/index.html");
    PA_OutputText(1,0,0,"r: %s",buffer2);
	while (1)
	{
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
