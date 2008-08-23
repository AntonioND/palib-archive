/* This example shows you how you can have a code run every VBL without notice of CPU time and all. 
Be aware that if your function is too long, it might slow down the DS or hang :/    
This can be used to have your own counters, or some strict important code running on its own  */


#include <PA9.h>


s32 MyCounter = 0;


// Custom function we'll use, with just a single counter for the sake of the example
void MyVBLFunction(void){

	MyCounter++; 

}


///Main function...
int main(void){


	//PAlib Init
	PA_Init();
	PA_InitVBL();
	
	PA_InitText(0,0); // On the bottom screen
		
	PA_VBLFunctionInit(MyVBLFunction); // Init the function to be executed every frame... You can remove it by using PA_VBLFunctionReset();
	
	while(1){  // No code beside the text output in the main loop, everything will be done in the VBL for once :)
	
		PA_OutputText(0, 2, 10, "Counter : %d", MyCounter); //    /60 to get seconds

	
		PA_WaitForVBL();
		//La boucle infinie du programme
	}
	return 0;
}
