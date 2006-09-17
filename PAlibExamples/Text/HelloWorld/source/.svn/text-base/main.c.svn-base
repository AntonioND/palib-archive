// Hello World function !!

#include <PA9.h>  // PAlib include


int main(void)	{

	// PAlib Inits
	PA_Init();
	PA_InitVBL();
	
	// Text Init
	PA_InitText(1, // Top screen...
				2);	// Use background number 2
	
	// Write some text...
	PA_OutputSimpleText(1,1,1,"Hello World !"); // Top screen, tile x = 1, y = 1...
	
	while(1) 	{  // Inifinite loop
		PA_WaitForVBL();
	}
	
	return 0;
}
