#ifndef _PA_jpeg
#define _PA_jpeg
#ifdef __cplusplus
extern "C" {
#endif
#include <PA.h>
extern void PA_jpeg_GetImageSize(char* name, int* width, int* height);
extern void PA_jpeg_BltImage(char* name, u16* vram, int output_width, int output_height);

#endif
