//Gfx converted using Mollusk's PAGfx Converter

//This file contains all the .c, for easier inclusion in a project

#ifdef __cplusplus
extern "C" {
#endif

#include "all_gfx.h"


// Background files : 
#include "bigfont.c"
#include "smallfont.c"

// Palette files : 
#include "bigfont.pal.c"
#include "smallfont.pal.c"

// Background Pointers :
PAGfx_struct bigfont = {(void*)bigfont_Map, 1024, (void*)bigfont_Tiles, 8256, (void*)bigfont_Pal, (int*)bigfont_Info };
PAGfx_struct smallfont = {(void*)smallfont_Map, 256, (void*)smallfont_Tiles, 3168, (void*)smallfont_Pal, (int*)smallfont_Info };


#ifdef __cplusplus
}
#endif

