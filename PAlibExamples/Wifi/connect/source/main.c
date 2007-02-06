// Includes
#include <PA9.h>       // Include for PA_Lib
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

	while (1)
	{
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
