#include <PA9.h>




///Main function...
int main(void){


	//PAlib Init
	PA_Init();
	PA_InitVBL();
	
	PA_InitText(1,0); // On the top screen
	
	PA_OutputSimpleText(1, 0, 10, "Waiting for Start to be pressed"); // Text
	
	PA_WaitFor(Pad.Newpress.Start);  // Wait for a specific thing...
	
	PA_OutputSimpleText(1, 0, 10, "Waiting for Stylus to be pressed"); // Text
	
	PA_WaitFor(Stylus.Newpress);  // Wait for a specific thing...
	
	PA_OutputSimpleText(1, 0, 10, "            All done !                "); // Text
	
	
	while(1){
	
	
	
		PA_WaitForVBL();
		//La boucle infinie du programme
	}
	return 0;
}
