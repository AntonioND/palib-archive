//Gfx converted using Mollusk's PAGfx Converter

//This file contains all the .c, for easier inclusion in a project

#ifdef __cplusplus
extern "C" {
#endif

#include "all_gfx.h"


// Background files : 
#include "mariobg.c"

// Sprite files : 
#include "mario.c"

// Palette files : 
#include "mariobg.pal.c"
#include "mario.pal.c"

// Background Pointers :
PAGfx_struct mariobg = {(void*)mariobg_Map, 768, (void*)mariobg_Tiles, 11840, (void*)mariobg_Pal, (int*)mariobg_Info };


#ifdef __cplusplus
}
#endif

