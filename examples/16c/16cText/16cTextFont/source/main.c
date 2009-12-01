#include <PA9.h>

// Include the newly converted fonts...
#include "all_gfx.h"

int main(){
	int i;

	PA_Init();

	// 16 color background init with default colors for text
	PA_Init16cBg(0, 3);
	PA_Init16cBg(1, 3);

	// Slots 0-4 are already used by default PAlib fonts, so we'll load on slots 5 and 6. 
	// Available slots : 5-9. You can overwrite a PAlib font though if you want to use slot 0 for example...
	PA_Add16cFont(5, &smallfont);
	PA_Add16cFont(6, &bigfont);	

	PA_16cText(1, 10, 25, 255, 40, "Small custom font...", 1, 5, 100);
	PA_16cText(1, 10, 40, 255, 60, "Big custom font...", 1, 6, 100);

	// Different colors on the bottom sreen...
	for (i = 0; i < 10; i++)
		PA_16cText(0, 10, i*20, 255, i*20 + 20, "Hello World", i+1, 6, 100);

	// Infinite loop to keep the program running
	while(true){
		PA_WaitForVBL();
	}
}