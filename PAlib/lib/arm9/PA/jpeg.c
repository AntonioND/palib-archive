#include "PA9.h"

void PA_jpeg_GetImageSize(char* name, int* width, int* height) {
  REG_EXEMEMCNT &= ~0x80;
  GBFS_FILE const* gbfs_file = 
    find_first_gbfs_file((void*)0x08000000);
  const unsigned char* image = (const unsigned char*)gbfs_get_obj(gbfs_file, 
				      name, 
				      0);
  JPEG_Decoder decoder;
  JPEG_Decoder_ReadHeaders(&decoder, &image);
  *width = decoder.frame.width;
  *height = decoder.frame.height;
  REG_EXEMEMCNT |= 0x80;
}

// Decode the jpeg file with the given name to the VRAM location
// specified.  The height and width are the height and width of the
// output bitmap.
void PA_jpeg_BltImage(char* name, u16* vram, int output_width, int output_height)
{
  REG_EXEMEMCNT &= ~0x80;
  GBFS_FILE const* gbfs_file = 
    find_first_gbfs_file((void*)0x08000000);
  uint8* image = (uint8*)gbfs_get_obj(gbfs_file, 
				      name, 
				      0);
  JPEG_DecompressImage(image, vram, output_width, output_height);

  REG_EXEMEMCNT |= 0x80;
}
