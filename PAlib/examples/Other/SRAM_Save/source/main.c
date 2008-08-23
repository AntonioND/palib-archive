/* This example will be about using the gba's SRAM
First thing, be carefull with SRAM : if someone is using WMB and has a gba cart (any game), you could
accidently erase his save !
To avoid this, we will check if we find our app's string, and if not ask if we can format...

All this will only work on hardawre !
*/


// Includes
#include <PA9.h>       // Include for PA_Lib


char string[20]; // This will be to check the string



// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	PA_InitText(1, 0);  // Initialise the text system on the top screen

	
	// First things first, we'll copy the 20 first characters to our string, to check if the right string is there
	PA_LoadData(1, // Take the data from slot 1. You should avoid slot 0, it may be corrupt
			string, // Copy the data to our array :)
			20);     // // Copy 20 characters...
	// PA_LoadData can be used to load anything : arrays, structures, etc... just put your variable and the size in bytes
	
	// Then we compare it to the string there should be :
	if (!PA_CompareText(string, "PA_Rulez")){ // If it isn't there... we'll ask if ok to format...
		PA_OutputSimpleText(1, 2, 2, "Sorry, but can I format your SRAM ? Press A if it's ok...");
		while (!Pad.Newpress.A) { // Now we wait for A to be pressed...
			PA_WaitForVBL();
		}
		// And now that's it ok to format, we format and then write the string in there...
		PA_SaveData(1, (void*)Blank, 30000); // This'll copy a whole block of zeros...
		
		// And now copy the string...
		PA_SaveString(1, "PA_Rulez");
		
		//We'll save the current time in memory to show it when we reset...
		PA_Save8bit(21, // save to slot 21
					PA_RTC.Hour);
		PA_Save8bit(22, PA_RTC.Minutes);
		
		PA_OutputSimpleText(1, 2, 2, "Please reset your DS to check that it worked...                            ");
	}
	else{ // If we are here, then the SRAM was already formatted, saves work !!
		PA_OutputSimpleText(1, 2, 2, "Your SRAM was correct...");
		
		// Since the Save worked, we'll tell when was the last time the DS was turned on with this demo
		PA_OutputText(1, 2, 4, "Last time : %02d:%02d", PA_Load8bit(21), PA_Load8bit(22)); // We use the Load function...
	}

	// Infinite loop to keep the program running
	while (1)
	{

		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
