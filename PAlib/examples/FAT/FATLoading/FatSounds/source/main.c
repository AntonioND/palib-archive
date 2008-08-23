#include <PA9.h>
#include <fat.h> // include for libfat


int main(int argc, char ** argv)
{
  PA_Init();
  PA_InitVBL();

  PA_InitText(0, 0);

  PA_OutputSimpleText(0, 5, 1, "FAT loading of sounds");

// Init ASlib before you play sounds
  AS_Init(AS_MODE_SURROUND | AS_MODE_16CH);
  AS_SetDefaultSettings(AS_PCM_8BIT, 11025, AS_NO_DELAY);

  fatInitDefault(); // Init for libfat. Automatically sets up DLDI and everything else you need for basic FAt access.

  PA_FatInitAllBuffers(); // Initialize all the memory buffers used by the FAT loading system

  PA_FatSetBasePath("DemoFiles/data");  // Set a base path from the card root to load your asset files from
                              // Within this base asset folder:
                              //      all background binaries should be in /bg/
                              //      sprite and sprite palette binaries in /sprites/
                              //      and RAW format sound files in /sfx/


// Load a couple RAW format sounds from card FAT into memory for playback later.
  PA_FatLoadSfx("SFX_1", // A reference name for the loaded sound
                "saberoff"); // The name of sound effect in EFS to load (minus the ".RAW" extension)
  PA_FatLoadSfx("SFX_2", "sfxb");


  while(1)
  {
    PA_OutputSimpleText(0, 0, 4, "Press A/B to play loaded sounds.");

    PA_OutputSimpleText(0, 0, 6, "Or press R to load other sounds.");

    if(Pad.Newpress.A) PA_FatPlaySfx("SFX_1"); // Play a sound file loaded with PA_FatLoadSfx using its reference name
    if(Pad.Newpress.B) PA_FatPlaySfx("SFX_2");

    if(Pad.Newpress.R)
    {
// You can unload a previously loaded sound file to free its slot in memory.
      PA_FatUnloadSfx("SFX_1"); // Unload a sound file using the reference name assigned with PA_FatLoadSfx
      PA_FatUnloadSfx("SFX_2");

// Now you can load some more.
      PA_FatLoadSfx("SFX_1", "laserpower");
      PA_FatLoadSfx("SFX_2", "boi-oing");
// Actually, since you have 32 sound slots, we didn't need to unload the previous ones, but this is a demo!

    }

    PA_WaitForVBL();
  }

  return 0;
}
