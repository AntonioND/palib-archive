// Includes
#include <PA9.h>

int main(int argc, char ** argv)
{
	int depth = 3;					// how deep to search for the folder
	char location[512];				// will hold the location of the TestDir folder
	strcpy(location, "press A");

	PA_Init();    		// Initializes PA_Lib
	PA_InitVBL(); 		// Initializes a standard VBL
	PA_InitText(1, 0);	// Initializes a debug text screen
	
	// Initialize libfat
	if (!fatInitDefault())
	{
		PA_OutputText(1, 1, 1, "Please patch this\nfile with a DLDI driver!");
	
		while (1)
			PA_WaitForVBL();
	}
	
	// Show usage
	PA_OutputText(1, 1, 1, "PA_Locate() example\nUse left / right to set\nthe search depth,\nstart with A.");

	// Infinite loop to keep the program running
	while (1)
	{
		if (Pad.Newpress.Right)
			depth++;
		else if (Pad.Newpress.Left && (depth > 0))
			depth--;
			
		if (Pad.Newpress.A)
		{
			if (!PA_Locate("/",			// search from the root
                           "TestDir",	// search for "TestDir"
                           true,			// the target is a directory
                           depth,		// use the specified depth
                           location))	// store the result in the location string
			{
				strcpy(location, "not found!");
			}
		}
	
		PA_OutputText(1, 1, 6, "Depth: %d  ", depth);
		PA_OutputText(1, 1, 7, "Result: %s               ", location);
	
		PA_WaitForVBL();
	}
	
	return 0;
}
