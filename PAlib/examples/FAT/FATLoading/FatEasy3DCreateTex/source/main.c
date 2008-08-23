
#include <PA9.h>
#include <fat.h> // include for libfat

int main(int argc, char ** argv)
{
  PA_Init();
  PA_InitVBL();

  PA_Init3D(); // Regular init for the 3D sprite system
  PA_Reset3DSprites();

  PA_InitText(0, 1);

  PA_OutputSimpleText(0, 4, 1, "Create texture from FAT");

  fatInitDefault(); // Init for libfat. Automatically sets up DLDI and everything else you need for basic FAT access.

  PA_FatInitAllBuffers(); // Initialize all the memory buffers used by the FAT loading system

  PA_FatSetBasePath("/DemoFiles/data");  // Set a base path from the card root to load your asset files from
                              // Within this base asset folder:
                              //      all background binaries should be in /bg/
                              //      2D or 3D sprite and their palette binaries in /sprites/
                              //      and RAW format sound files in /sfx/


// Load a sprite image from FAT to VRAM. It works just like PA_3DCreateTex!
  u16 gfx = PA_FatEasy3DCreateTex("pokekun", // Name of a texture binary created with PAGfx (without "_Texture.bin")
                                  32, // Texture width
                                  64, // Texture height
                                  TEX_256COL); // 256 color texture (see TEX_16COL, TEX_4COL, etc.)


// Create a palette for the 3D sprites that will be created later. It works just like PA_Load3DSpritePal!
  PA_FatEasyLoad3DSpritePal(0, // Sprite palette number
                            "pokekun"); // Name of a palette binary in EFS created by PAGfx (without the "_Pal.bin")


  PA_OutputSimpleText(0, 1, 22, "Press A to create a 3D sprite!");

  u16 nsprites0 = 0; // Number of 3D sprites created

  while(1)
  {

    if (Pad.Newpress.A && nsprites0 < 1024) // You can have 1024 3D sprites!!!
    {
// Since the sprite is already in VRAM you can just use the normal PA_3DCreateSpriteFromTex function
      PA_3DCreateSpriteFromTex(nsprites0, // Next sprite to load...
                               gfx, // texture in VRAM to use, no image copying !
                               32, 64, 0, PA_RandMax(240), PA_RandMax(160)); // The rest is like normal 3D sprites

      ++nsprites0; // Next time, load the next sprite number
    }

    PA_WaitForVBL();
    PA_3DProcess();
  }

  return 0;
}
