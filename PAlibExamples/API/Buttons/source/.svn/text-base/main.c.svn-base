/* API system, example : Buttons...
Now this can be very useful ! It creates buttons of the desired size, and automatically checks for presses... A given function is executed each time you press the button

*/

// Includes
#include <PA9.h>       // Include for PA_Lib

u8 line = 0; // Debug purpose, you'll see

void Test(void){ // Function to execute when the button0 is pressed...
	PA_OutputSimpleText(1, 0, line, "Button 0 pressed");
	line++; // Next time, on the next line...
}
void Test2(void){ // Function to execute when the button1 is pressed...
	PA_OutputSimpleText(1, 0, line, "Other button pressed");
	line++; // Next time, on the next line...
}
void Test3(void){ // Function to execute when the button1 is pressed...
	PA_OutputSimpleText(1, 0, line, "Third button pressed");
	line++; // Next time, on the next line...
}


// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	PA_InitText(1, 0); PA_SetTextCol(1, 31, 0, 0); // Red text
	
	// Init 8 bit backgrounds on both screens
	PA_Init8bitBg(0, 3);
	PAPI_Init(0); // Init the API on the bottom screen
	

	PAPI_CreateButton(0, // screen
					  20, // X position
					  20, // Y position
					  32, // X size
					  32, // Y size
					  &Test, // function to execute on button press
					  "Test", // Text displayed on the button
					  0, // Text palette color, we'll put 0 for transparent
					  1); // Text size (you must enable all text sizes in PA9.h to use this)
	PAPI_CreateButton(0, 120, 40, 64, 16, &Test2, "Other button", 0, 1);
	PAPI_CreateButton(0, 120, 0, 64, 32, &Test3, "Third button", 0, 1); 	
	
	
	// Infinite loop to keep the program running
	while (1){
	
		PAPI_CheckButton(); // Check the buttons
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
