//Gfx converted using Mollusk's PAGfx Converter

//This file contains all the .h, for easier inclusion in a project

#ifndef ALL_GFX_H
#define ALL_GFX_H


// Sprite files : 
extern const unsigned char mario_Sprite[3072] __attribute__ ((aligned (4))) ;  // Pal : sprite0_Pal

// Background files : 
extern const int mario_world_Info[3]; // BgMode, Width, Height
extern const unsigned short mario_world_Map[768] __attribute__ ((aligned (4))) ;  // Pal : mario_world_Pal
extern const unsigned char mario_world_Tiles[768] __attribute__ ((aligned (4))) ;  // Pal : mario_world_Pal

extern const int back_Info[3]; // BgMode, Width, Height
extern const unsigned short back_Map[768] __attribute__ ((aligned (4))) ;  // Pal : back_Pal
extern const unsigned char back_Tiles[256] __attribute__ ((aligned (4))) ;  // Pal : back_Pal


// Palette files : 
extern const unsigned short sprite0_Pal[20] __attribute__ ((aligned (4))) ;
extern const unsigned short mario_world_Pal[7] __attribute__ ((aligned (4))) ;
extern const unsigned short back_Pal[6] __attribute__ ((aligned (4))) ;


#endif

