//Gfx converted using Mollusk's PAGfx Converter

//This file contains all the .c, for easier inclusion in a project

#ifdef __cplusplus
extern "C" {
#endif

#include "all_gfx.h"


// Background files : 
#include "PA_text.c"
#include "PA_text2.c"

// Palette files : 
#include "PA_text.pal.c"
#include "PA_text2.pal.c"

// Background Pointers :
PAGfx_struct PA_text = {(void*)PA_text_Map, 768, (void*)PA_text_Tiles, 6080, (void*)PA_text_Pal, (int*)PA_text_Info };
PAGfx_struct PA_text2 = {(void*)PA_text2_Map, 768, (void*)PA_text2_Tiles, 11200, (void*)PA_text2_Pal, (int*)PA_text2_Info };


#ifdef __cplusplus
}
#endif

