/* FAT functions....
*/

// Includes
#include <PA9.h>       // Include for PA_Lib
#include <fat.h>
#include <sys/dir.h>

// Function: main()
int main(int argc, char ** argv)
{
	PA_Init();    // Initializes PA_Lib
	PA_InitVBL(); // Initializes a standard VBL
	
	PA_InitText(0, 0);  // Initialise the text system on the bottom screen	
	
	PA_InitText(1, 0);  // Initialise the text system on the top screen
	
	PA_WaitForVBL();  PA_WaitForVBL();  PA_WaitForVBL();  // wait a few VBLs
	
	fatInitDefault(); //Initialise fat library

	// The following code was pulled from 
	// http://chishm.drunkencoders.com/libfat/index.html
	// It has been slightly modified...
	int linenumber =0; 
	int screen = 1;

	struct stat st;
	char filename[256]; // to hold a full filename and string terminator
	DIR_ITER* dir = diropen("/");

	if (dir == NULL) 
	{
		PA_OutputText(1,2,2,"Unable to open the directory.");
	} 
	else 
	while (dirnext(dir, filename, &st) == 0) 
	{
	   if (linenumber ==24) //if we hit this twice we are overwriting on the bottom screen :(
		{ 
		   screen=0; //Output on bottom if we filled the top screen
		   linenumber = 0; //reset line number...
		}   
		// st.st_mode & S_IFDIR indicates a directory
		PA_OutputText(screen,0,linenumber, "%02d%s: %s\n", linenumber, (st.st_mode & S_IFDIR ? "D" : "-"), filename);
		linenumber++;//next line
	}
	//nothing to do but wait....
	while (1)
	{ 
			
		PA_WaitForVBL();
	}
	
	return 0;
} // End of main()












