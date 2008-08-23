#include <PA9.h>
#include <fat.h> // include for libfat


int main(int argc, char ** argv)
{
  PA_Init();
  PA_InitVBL();

  fatInitDefault(); // Init for libfat. Automatically sets up DLDI and everything else you need for basic FAT access.

  PA_FatInitAllBuffers(); // Initialize all the memory buffers used by the FAT loading system

  PA_FatSetBasePath("/DemoFiles/data");  // Set a base path from the card root to load your asset files from
                              // Within this base asset folder:
                              //      all background binaries should be in /bg/
                              //      sprite and sprite palette binaries in /sprites/
                              //      and RAW format sound files in /sfx/


// First we'll load a regular 256x192 background converted with PAGfx... 
  PA_FatEasyBgLoad(1, // Screen
                   3, // Background number (0-3)
                   "smb"); // Name of the background to load

// Next, we'll load a large map (2048x2048). It's actually the same.
  PA_FatEasyBgLoad(0, 3, "WorldMap");

// Some variables for scrolling around the background...
  s32 scrollx = 0; 
  s32 scrolly = 0;

  while (1)
  {

    if(Stylus.Held)
    {
      scrollx -= Stylus.Vx; // Scroll sround according to the stylus
      scrolly -= Stylus.Vy;
    }

    PA_LargeScrollXY(0, // Screen
                     3, // Background number
                     scrollx, // X scroll
                     scrolly); // and Y scroll

    PA_WaitForVBL();
  }

  return 0;
} // End of main()
