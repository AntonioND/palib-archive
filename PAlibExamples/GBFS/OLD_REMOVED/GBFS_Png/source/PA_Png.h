#ifndef PA_PNG
#define PA_PNG


#include "libpng/png.h"


/* Errors for read_png(); */

#define     READPNG_SUCCESS                     0
#define     READPNG_BADSIG                      1
#define     READPNG_CREATEREADSTRUCT_ERROR      2
#define     READPNG_CREATEINFOSTRUCT_ERROR      3
#define     READPNG_INITIO_ERROR                4
#define     READPNG_READIMG_ERROR               5

/* Flags for read_png(); */

#define     READPNG_WITH_PALETTE                1

/* Macros */

#define     GETRED24(p)                    (p)
#define     GETGREEN24(p)                  (p+1)
#define     GETBLUE24(p)                   (p+2)
#define     GETALPHA24(p)                  (p+3)
#define     GETPIXEL24(p,x,y,w,c)          (p+(x+y*w)*c)

/*#define     RGBA_GETRED24(p)                    (p)
#define     RGBA_GETGREEN24(p)                  (p+1)
#define     RGBA_GETBLUE24(p)                   (p+2)
#define     RGBA_GETALPHA24(p)                  (p+3)
#define     RGBA_GETPIXEL24(p,x,y,sx)           (p+(x+y*sx)*4)*/

#define     PNG_IS_PALETTE(t)                   (t & PNG_COLOR_MASK_PALETTE)
#define     PNG_IS_RGB(t)                       (t & PNG_COLOR_MASK_COLOR)
#define     PNG_IS_RGBA(t)                      (t & PNG_COLOR_MASK_ALPHA)

/* (Declared static) void read_png_data(png_structp png_ptr, png_bytep data, png_size_t length); */
int read_png(png_byte **png_out, png_info **png_outinfo, void *png_file, unsigned long png_filesize, int flags);
void png_read_free(png_byte *png_out, png_info *png_outinfo);



#endif

