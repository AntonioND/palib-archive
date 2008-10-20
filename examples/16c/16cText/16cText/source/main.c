// Includes
#include <PA9.h>       // Include for PA_Lib





// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	

	
	PA_Init16cBg(0, 3); 	PA_Init16cBg(1, 3);  // 16 color background init with default colors for text

	PA_16cText(1, //screen 
					10, 10, 255, 20, //x1, y1, x2, y2 position
					"Hello World", //text
					1, //color (1-10)
					0, // text size (0-4)
					100); // maximum number of characters (use like 10000 if you don't know)
	PA_16cText(1, 10, 25, 255, 40, "Hello World", 1, 1, 100);
	PA_16cText(1, 10, 40, 255, 60, "Hello World", 1, 2, 100);
	PA_16cText(1, 10, 60, 255, 80, "Hello World", 1, 3, 100);
	PA_16cText(1, 10, 80, 255, 100, "Hello World", 1, 4, 100);
	
	u8 i;

	// Different colors on the bottom sreen...
	for (i = 0; i < 10; i++){
		PA_16cText(0, 10, i*20, 255, i*20 + 20, "Hello World", i+1, 3, 100);
	}		
	
	// Infinite loop to keep the program running
	while (1)
	{
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
