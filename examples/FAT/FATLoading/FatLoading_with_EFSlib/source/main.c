#include <PA9.h>

#include "efs_lib.h" // be sure to put the efs_lib.c/.h files in your source diectory and #include


int main(int argc, char ** argv)
{
  PA_Init();
  PA_InitVBL();

  PA_InitText(0, 0);

  PA_OutputSimpleText(0, 4, 1, "FAT loading with EFS");

// Init ASlib before you play sounds
  AS_Init(AS_MODE_SURROUND | AS_MODE_16CH);
  AS_SetDefaultSettings(AS_PCM_8BIT, 11025, AS_NO_DELAY);

  PA_OutputSimpleText(0, 0, 3, "Initing the EFS. Please wait..."); // Always good to give the user some warning before initing EFS!
  if(EFS_Init(EFS_AND_FAT | EFS_DEFAULT_DEVICE, NULL)) { // Standard EFS and libfat initialization
  PA_OutputSimpleText(0, 0, 4, "EFS init okay!");


  PA_FatInitAllBuffers(); // Initialize all the memory buffers used by the FAT loading system

  PA_FatSetBasePath("data");  // Set a base path from the EFS root to load your asset files from
                              // Within this base asset folder:
                              //      all background binaries should be in /bg/
                              //      2D and 3D sprite and palette binaries in /sprites/
                              //      and RAW format sound files in /sfx/


// Load some RAW format sounds from EFS into memory for playback later.
  PA_FatLoadSfx("SFX_1", // A reference name for the loaded sound
                "saberoff"); // The name of sound effect in EFS to load (minus the ".RAW" extension)
  PA_FatLoadSfx("SFX_2", "sfxb");
  PA_FatLoadSfx("SFX_3", "laserpower");
  PA_FatLoadSfx("SFX_4", "boi-oing");
//  You have 32 sound effect slots to work with, but you can also unload with PA_UnloadSfx and re-use a slot


// Create some palettes for sprites that will be created later
  PA_FatEasyLoadSpritePal(1, // Screen
                          0, // Sprite palette number
                          "harry"); // Name of a palette binary in EFS created by PAGfx (without the "_Pal.bin")
  PA_FatEasyLoadSpritePal(1, 1, "mario");
  PA_FatEasyLoadSpritePal(1, 2, "sonic");
  PA_FatEasyLoadSpritePal(1, 3, "pokekun");

  u8 mode = 0;

  while(1)
  {
    mode += Pad.Newpress.R;
    if(mode==3) mode = 0;

    if(mode == 0)
    {
      PA_OutputSimpleText(0, 0, 8, "You can play sounds from EFS!   ");
      PA_OutputSimpleText(0, 0, 20, "Press A/B/X/Y to play sounds.   ");
      PA_OutputSimpleText(0, 0, 21, "Or press R to try backgrounds.  ");

      if(Pad.Newpress.A) PA_FatPlaySfx("SFX_1"); // Play a sound file loaded with PA_FatLoadSfx using it's reference name
      if(Pad.Newpress.B) PA_FatPlaySfx("SFX_2");
      if(Pad.Newpress.X) PA_FatPlaySfx("SFX_3");
      if(Pad.Newpress.Y) PA_FatPlaySfx("SFX_4");
    }

    if(mode == 1)
    {
      PA_OutputSimpleText(0, 0, 8, "Load backgrounds from EFS!      ");
      PA_OutputSimpleText(0, 0, 20, "Press A/B/X/Y to display bgs    ");
      PA_OutputSimpleText(0, 0, 21, "Or press R to try sprites.      ");

// Load a background directly from EFS. It works just like PA_EasyBgLoad!
      if(Pad.Newpress.A) PA_FatEasyBgLoad(1, // Screen
                                          3, // Background number
                                          "pitfall"); // Name of a background with PAGFx created binaries in EFS
      if(Pad.Newpress.B) PA_FatEasyBgLoad(1, 3, "smb");
      if(Pad.Newpress.X) PA_FatEasyBgLoad(1, 3, "sonic1");
      if(Pad.Newpress.Y) PA_FatEasyBgLoad(1, 3, "ganbare");

      if(Pad.Newpress.Start) PA_ResetBgSysScreen(1);
    }

    if(mode == 2)
    {
      PA_OutputSimpleText(0, 0, 8, "You can load sprites from EFS!  ");
      PA_OutputSimpleText(0, 0, 20, "Press A/B/X/Y to create sprites ");
      PA_OutputSimpleText(0, 0, 21, "Or press R to try sounds.       ");

// Sprite loading from EFS using palettes loaded earlier. It works just like PA_CreateSprite!
      if(Pad.Newpress.A) PA_FatEasyCreateSprite(1, // Screen
                                                0, // Sprite number
                                                "harry", // Name of PAGfx created sprite binary (without the "_Sprite.bin")
                                                OBJ_SIZE_32X32, // Sprite size macro
                                                1, // 16 color mode ('1' for 256 color mode)
                                                0, // Sprite palette number
                                                PA_RandMax(223), PA_RandMax(159)); // X and Y position on the screen
      if(Pad.Newpress.B) PA_FatEasyCreateSprite(1, 1, "mario", OBJ_SIZE_16X16, 1, 1, PA_RandMax(191), PA_RandMax(127));
      if(Pad.Newpress.X) PA_FatEasyCreateSprite(1, 2, "sonic", OBJ_SIZE_32X32, 1, 2, PA_RandMax(239), PA_RandMax(175));
      if(Pad.Newpress.Y) PA_FatEasyCreateSprite(1, 3, "pokekun", OBJ_SIZE_32X64, 1, 3, PA_RandMax(223), PA_RandMax(159));
    }

    PA_WaitForVBL();
  }

  }
  else PA_OutputSimpleText(0, 0, 3, "EFS init failed.");

  while(1) {PA_WaitForVBL();}

  return 0;
}
