//Gfx converted using Mollusk's PAGfx Converter

//This file contains all the .c, for easier inclusion in a project

#ifdef __cplusplus
extern "C" {
#endif

#include "all_gfx.h"


// Background files : 
#include "bg0.c"
#include "bg1.c"
#include "bg2.c"
#include "bg3.c"

// Palette files : 
#include "bg0.pal.c"
#include "bg1.pal.c"
#include "bg2.pal.c"
#include "bg3.pal.c"

// Background Pointers :
PAGfx_struct bg0 = {(void*)bg0_Map, 768, (void*)bg0_Tiles, 12352, (void*)bg0_Pal, (int*)bg0_Info };
PAGfx_struct bg1 = {(void*)bg1_Map, 768, (void*)bg1_Tiles, 11008, (void*)bg1_Pal, (int*)bg1_Info };
PAGfx_struct bg2 = {(void*)bg2_Map, 768, (void*)bg2_Tiles, 11968, (void*)bg2_Pal, (int*)bg2_Info };
PAGfx_struct bg3 = {(void*)bg3_Map, 768, (void*)bg3_Tiles, 11264, (void*)bg3_Pal, (int*)bg3_Info };


#ifdef __cplusplus
}
#endif

