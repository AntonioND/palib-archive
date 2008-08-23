#include <PA9.h>




///Main function...
int main(void){


	//PAlib Init
	PA_Init();
	PA_InitVBL();
	
	PA_InitText(1,0); // On the top screen
	PA_InitText(0,0); // On the bottom screen
		
	u8 ncounters = 1; // Number of counters active !
	
	PA_OutputSimpleText(1, 2, 10, "Press Start to start a new counter"); // Text
	PA_OutputSimpleText(0, 2, 0, "Counters :"); // Text
	u8 i;
	
	PA_VBLCounterStart(0); // Start the first counter :)
	
	while(1){
	
		if(Pad.Newpress.Start&&(ncounters<16)){ // When Start pressed, max 16 counters
			PA_VBLCounterStart(ncounters); // Start a new counter with the latest number
			ncounters++; 
		}
		
		for (i = 0; i < ncounters; i++){ // Thing to do every time for each counter active :
			PA_OutputText(0, 0, 2+i, "%02d : %d VBLs, %d seconds", i, PA_VBLCounter[i], PA_VBLCounter[i]/60); //    /60 to get seconds
		}
	
		PA_WaitForVBL();
		//La boucle infinie du programme
	}
	return 0;
}
