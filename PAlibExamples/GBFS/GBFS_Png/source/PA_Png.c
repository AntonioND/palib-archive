#include "PA_Png.h"
#include "libpng/png.h"

#include <stdlib.h>

/* Structure passée en "argument" à la fonction de lecture */
struct png_read_data_str
{
void *data;
unsigned long filesize;
unsigned long filepos;
};

static void read_png_data(png_structp png_ptr, png_bytep data, png_size_t length)
{

   void *pngf_data = ((struct png_read_data_str*)png_ptr->io_ptr)->data;
   unsigned long pngf_filesize = ((struct png_read_data_str*)png_ptr->io_ptr)->filesize;
   unsigned long pngf_filepos = ((struct png_read_data_str*)png_ptr->io_ptr)->filepos;

   if((pngf_filepos + length) < pngf_filesize)
   {
	   memcpy(data, pngf_data + pngf_filepos, length);
   }
   else
   {
	   memcpy(data, pngf_data + pngf_filepos, pngf_filesize - pngf_filepos);
   }

   ((struct png_read_data_str*)png_ptr->io_ptr)->filepos += length;

}

int read_png(png_byte **png_out, png_info **png_outinfo, void *png_file, unsigned long png_filesize, int flags)
{
   int y;
   png_structp png_ptr;
   png_infop info_ptr;
   png_bytepp row_pointers;
   png_bytep row_data;
   struct png_read_data_str read_info;


   /* Fill read structure */
   read_info.data = png_file;
   read_info.filesize = png_filesize;
   read_info.filepos = 8;

   /* Check if png_file has a correct PNG signature */
   if (png_sig_cmp(png_file, 0, 8))
   {
      return READPNG_BADSIG;
   }

   /* Create read structure */
   png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

   if (!png_ptr)
   {
      return READPNG_CREATEREADSTRUCT_ERROR;
   }

   /* Create info structure */
   info_ptr = png_create_info_struct(png_ptr);

   if (!info_ptr)
   {
      return READPNG_CREATEINFOSTRUCT_ERROR;
   }

   if (setjmp(png_jmpbuf(png_ptr)))
   {
      return READPNG_INITIO_ERROR;
   }

   /* Set user IO function for reading ... */
   png_set_read_fn(png_ptr, (png_voidp)&read_info, read_png_data);

   /* Set size of signature */
   png_set_sig_bytes(png_ptr, 8);

   /* Read informations about image */
   png_read_info(png_ptr, info_ptr);

   /* Set transformations to do on the image */


   if (info_ptr->bit_depth == 16)   /* 16 bits to 24 */
      png_set_strip_16(png_ptr);
   if (info_ptr->color_type == PNG_COLOR_TYPE_PALETTE && !(flags & READPNG_WITH_PALETTE))    /* If needed, ignore palette */
      png_set_palette_to_rgb(png_ptr);
   if (info_ptr->bit_depth < 8)     /* < 8 bits to 24 */
      png_set_expand(png_ptr);
   if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))  /* ? */
      png_set_expand(png_ptr);
   if (info_ptr->color_type == PNG_COLOR_TYPE_GRAY || info_ptr->color_type == PNG_COLOR_TYPE_GRAY_ALPHA)    /* Grayscale to RVB */
      png_set_gray_to_rgb(png_ptr);

   /* Apply transformations */
   png_read_update_info(png_ptr, info_ptr);

   if (setjmp(png_jmpbuf(png_ptr)))
   {
      return READPNG_READIMG_ERROR;
   }

   /* Allocate array for image's pixels */
   row_pointers = (png_bytepp) malloc(sizeof(png_byte*) * info_ptr->height);

   if(*png_out)
   {
      row_data = (png_bytep) *png_out;
   }
   else
   {
      row_data = (png_bytep) malloc(info_ptr->rowbytes * info_ptr->height);
      *png_out = row_data;
   }
   
   for (y=0; y < info_ptr->height; y++)
   {
      row_pointers[y] = (png_bytep) row_data + y * info_ptr->rowbytes;
   }


   /* Read PNG file */
   png_read_image(png_ptr, row_pointers);

   /* Free structures */
   png_destroy_read_struct(&png_ptr, NULL, NULL);
   free(row_pointers);

   /* Give adress to the information structure */
   *png_outinfo = info_ptr;

   /* Success !!! */
   return READPNG_SUCCESS;

}

void read_png_free(png_byte *png_out, png_info *png_outinfo)
{
   png_destroy_read_struct(NULL, &png_outinfo, NULL);
   free(png_out);
}


