//Gfx converted using Mollusk's PAGfx Converter

//This file contains all the .c, for easier inclusion in a project

#ifdef __cplusplus
extern "C" {
#endif

#include "all_gfx.h"


// Background files : 
#include "Town.c"

// Palette files : 
#include "Town.pal.c"

// Background Pointers :
PAGfx_struct Town = {(void*)Town_Map, 16384, (void*)Town_Tiles, 114560, (void*)Town_Pal, (int*)Town_Info };


#ifdef __cplusplus
}
#endif

