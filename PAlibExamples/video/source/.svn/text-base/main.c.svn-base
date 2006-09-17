// Load an 16 bit jpeg on the screen !
// The jpeg must be put in the data folder...



// Includes
#include <PA9.h>       // Include for PA_Lib
#include <stdio.h>
#include <stdlib.h>
#include "neosplash.h"
// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	int nfiles = PA_InitGBFS();
	s8 videos[255];
while(1){
PA_ResetBgSys();
	PA_Init16bitBg(0, 3);
	PA_Init16bitBg(1, 3);
	PA_InitText(0, 0);
	PA_InitText(1, 0);
	PA_LoadJpeg(0, (void*)neosplash);
		int i, mauvais=0, maxvideo=-1;
	for (i = 0; (i < nfiles); i++){
	if(PA_GBFSfile[i].Ext[0]=='h'){
	videos[i-mauvais]=i;
 PA_OutputText(1, 0, i-mauvais, "  %s  ", correctname(PA_GBFSfile[i].Name));
 maxvideo++;
 }
else mauvais++;
}
	i=0;
	int numvideo=0;
    while(!Pad.Held.A&&!Pad.Held.Start){
    numvideo-=Pad.Newpress.Up;
    numvideo+=Pad.Newpress.Down;
    if(numvideo>maxvideo)numvideo=maxvideo;
    if(numvideo<0)numvideo=0;
    PA_OutputText(1,0,numvideo,">");
    PA_WaitForVBL();
    PA_OutputText(1,0,numvideo," ");
    }
    for (i = 0; (i < nfiles); i++)PA_OutputText(1, 0, i-mauvais, "                          ");
    PA_OutputText(1, 10, 15, "loading");
	// Load the jpeg on the given screen... pretty simple !	
	PA_OutputText(1, 10, 15, "              ");	
	PA_LoadMultiVidGBFS(videos[numvideo]);
    }
	// Infinite loop to keep the program running
	
	return 0;
} // End of main()
