//Gfx converted using Mollusk's PAGfx Converter

//This file contains all the .c, for easier inclusion in a project

#ifdef __cplusplus
extern "C" {
#endif

#include "all_gfx.h"


// Background files : 
#include "WorldMap.c"
#include "smb.c"

// Palette files : 
#include "WorldMap.pal.c"
#include "smb.pal.c"

// Background Pointers :
PAGfx_struct WorldMap = {(void*)WorldMap_Map, 65536, (void*)WorldMap_Tiles, 15552, (void*)WorldMap_Pal, (int*)WorldMap_Info };
PAGfx_struct smb = {(void*)smb_Map, 768, (void*)smb_Tiles, 4224, (void*)smb_Pal, (int*)smb_Info };


#ifdef __cplusplus
}
#endif

