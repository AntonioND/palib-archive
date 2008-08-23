// This is basicly an example using the brightness to fade in/out...



// Includes
#include <PA9.h>       // Include for PA_Lib

#include "test.h"
s16 level = 0;
s16 fade;
u8 black = 0;

void HBL_function(void){
   s16 vcount = PA_GetVcount();
   vcount++;
   if(vcount > 192) vcount = 0; // Get correct vcount
   fade = (vcount+(level*4)-192)>>2;   
   if(fade < 0) fade = 0;
   if(fade > 31) fade = 31;
   
   if(black) PA_SetBrightness(0, fade-31);
   else PA_SetBrightness(0, 31-fade);
}   


// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	PA_InitText(1, 0);
	
	PA_SetBrightness(0, 31);
	
	PA_Init8bitBg(0, 3);
	PA_LoadGif(0, (void*)test);
	


	irqSet(IRQ_HBLANK, HBL_function);
	irqEnable(IRQ_HBLANK);
	
	u8 i;
	level = 0; 
	while(1){
	   PA_OutputText(1, 8, 10, "Fading in, white  ");
		for(level = 0; level < 80; level++) PA_WaitForVBL();
		for(i = 0; i < 60; i++) PA_WaitForVBL(); // Wait 1 second
		
		black = 1;
		PA_OutputText(1, 8, 10, "Fading out, black ");
		for(level = 80; level > 0; level--) PA_WaitForVBL();	
		for(i = 0; i < 60; i++) PA_WaitForVBL(); // Wait 1 second
		
	   PA_OutputText(1, 8, 10, "Fading in, black  ");
		for(level = 0; level < 80; level++) PA_WaitForVBL();
		for(i = 0; i < 60; i++) PA_WaitForVBL(); // Wait 1 second				
		
		black = 0;		
		PA_OutputText(1, 8, 10, "Fading out, white ");
		for(level = 80; level > 0; level--) PA_WaitForVBL();
		for(i = 0; i < 60; i++) PA_WaitForVBL(); // Wait 1 second		
	}		

	return 0;
} // End of main()
