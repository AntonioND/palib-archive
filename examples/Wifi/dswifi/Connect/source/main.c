// Basic dswifi lib example using PAlib
//
// Make sure you have selected an ARM7 binary with DSWIFI support in your project Makefile!
//

// Includes
#include <PA9.h>       // Include for PA_Lib
// Function: main()
int main()
{
	PA_Init();    // Initializes PA_Lib
	
	// Initialise the text system on the top screen
	PA_LoadDefaultText(0,0);
	PA_LoadDefaultText(1, 0);
	
	PA_OutputText(0, 1, 1, "PA_InitWifi() ...");
    PA_InitWifi();
	PA_OutputText(0, 1, 2, "done!\nPA_ConnectWifiWFC() ...");
    if (!PA_ConnectWifiWFC())
	{
		PA_OutputText(0, 1, 4, "error!");
		return 1;
	}
    PA_OutputText(0, 1, 4, "done!\nWIFI is now ready!");

	while (1)
	{
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()