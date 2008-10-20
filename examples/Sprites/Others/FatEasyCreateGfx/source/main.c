#include <PA9.h>
#include <fat.h> // include for libfat


int main(int argc, char ** argv)
{
  PA_Init();
  PA_InitVBL();

  PA_InitText(0, 0);

  PA_OutputSimpleText(0, 6, 1, "Create GFX from FAT");

  fatInitDefault(); // Init for libfat. Automatically sets up DLDI and everything else you need for basic FAT access.

  PA_FatInitAllBuffers(); // Initialize all the memory buffers used by the FAT loading system

  PA_FatSetBasePath("/DemoFiles/data");  // Set a base path from the card root to load your asset files from
                              // Within this base asset folder:
                              //      all background binaries should be in /bg/
                              //      2D and 3D sprite and palette binaries in /sprites/
                              //      and RAW format sound files in /sfx/


  u16 gfx[2];

// Load a sprite image from FAT to VRAM. It works just like PA_CreateGfx!
  gfx[0] = PA_FatEasyCreateGfx(0, // Screen
                               "pokekun", // Name of a sprite binary created with PAGfx (without the "_Sprite.bin")
                               OBJ_SIZE_32X64, // Sprite size macro
                               1); // 256 color sprite

// Let's make another one for the top screen 
  gfx[1] = PA_FatEasyCreateGfx(1, "pokekun", OBJ_SIZE_32X64, 1);


// Create some palettes for sprites that will be created later. It works just like PA_LoadSpritePal!
  PA_FatEasyLoadSpritePal(0, // Screen
                          0, // Sprite palette number
                          "pokekun"); // Name of a palette binary in EFS created by PAGfx (without the "_Pal.bin")

// for the top screen, too
  PA_FatEasyLoadSpritePal(1, 0, "pokekun");


  PA_OutputSimpleText(0, 0, 20, "Press A to create a sprite on\nthe bottom screen, B for one on\nthe top screen");

  u8 nsprites0 = 0; // Number of sprites on the bottom screen
  u8 nsprites1 = 0; // Top screen

  while(1)
  {

    if (Pad.Newpress.A && nsprites0 < 128)
    {
// Since the sprite is already in VRAM you can just the normal PA_CreateSpriteFromGfx
      PA_CreateSpriteFromGfx(0, // screen
                             nsprites0, // Next sprite to load...
                             gfx[0], // image to use in memory, no image copying !
                             OBJ_SIZE_32X64, 1, 0, PA_RandMax(224), PA_RandMax(128)); // The rest is like normal sprites

      ++nsprites0; // Next time, load the next sprite number
    }

    if (Pad.Newpress.B && nsprites1 < 128)
    {
      PA_CreateSpriteFromGfx(1, nsprites1, gfx[1], OBJ_SIZE_32X64, 1, 0, PA_RandMax(224), PA_RandMax(128));
      ++nsprites1;
    }

    PA_WaitForVBL();

  }

  return 0;
}
