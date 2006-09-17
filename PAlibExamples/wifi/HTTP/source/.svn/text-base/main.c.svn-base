// Includes
#include <PA9.h>       // Include for PA_Lib
#include <stdio.h>

// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
    PA_InitWifi();
    PA_ConnectWifiWFC();
    
	// Initialise the text system on the top screen
	PA_InitText(0,0);
	PA_InitText(1, 0);
	int sock;
	char *buffer2 = malloc(256*256);

    PA_GetHTTP(buffer2, "http://www.google.be/index.html");
    PA_OutputText(1,0,0,"r: %s",buffer2);
	while (1)
	{
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
