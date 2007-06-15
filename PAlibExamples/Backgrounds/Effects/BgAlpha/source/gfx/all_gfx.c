//Gfx converted using Mollusk's PAGfx Converter

//This file contains all the .c, for easier inclusion in a project

#ifdef __cplusplus
extern "C" {
#endif

#include "all_gfx.h"


// Background files : 
#include "bg0.c"
#include "bg1.c"

// Palette files : 
#include "bg0.pal.c"
#include "bg1.pal.c"

// Background Pointers :
PAGfx_struct bg0 = {(void*)bg0_Map, 768, (void*)bg0_Tiles, 3776, (void*)bg0_Pal, (int*)bg0_Info };
PAGfx_struct bg1 = {(void*)bg1_Map, 768, (void*)bg1_Tiles, 256, (void*)bg1_Pal, (int*)bg1_Info };


#ifdef __cplusplus
}
#endif

