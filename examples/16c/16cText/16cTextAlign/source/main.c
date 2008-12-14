// Includes
#include <PA9.h>       // Include for PA_Lib



// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	PA_InitText(0, 0);
	
	PA_Init16cBg(0, 3); 	PA_Init16cBg(1, 3);  // 16 color background init with default colors for text
	
	PA_16cTextAlign(ALIGN_LEFT);
	PA_16cText(1, 0, 10, 255, 50, "Left aligned text, blabla, blabla, life is life, and if you don't like PAlib, just don't use it and stop ditching... More blabla to make the text long enough to test the text system.", 1, 2, 1000);
	
	PA_16cTextAlign(ALIGN_RIGHT);
	PA_16cText(1, 0, 50, 255, 90, "Right aligned text, blabla, blabla, life is life, and if you don't like PAlib, just don't use it and stop ditching... More blabla to make the text long enough to test the text system.", 1, 2, 1000);
	
	PA_16cTextAlign(ALIGN_CENTER);
	PA_16cText(1, 0, 90, 255, 130, "Centered text, blabla, blabla, life is life, and if you don't like PAlib, just don't use it and stop ditching... More blabla to make the text long enough to test the text system.", 1, 2, 1000);
	
	PA_16cTextAlign(ALIGN_JUSTIFY);
	PA_16cText(1, 0, 130, 255, 180, "Justified text, blabla, blabla, life is life, and if you don't like PAlib, just don't use it and stop ditching... More blabla to make the text long enough to test the text system.", 1, 2, 1000);

	// Infinite loop to keep the program running
	while (1)
	{
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()
