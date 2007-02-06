//Gfx converted using Mollusk's PAGfx Converter

//This file contains all the .h, for easier inclusion in a project

#ifndef ALL_GFX_H
#define ALL_GFX_H


// Background files : 
extern const int Town_Info[3]; // BgMode, Width, Height
extern const unsigned int Town_Map[16384] __attribute__ ((aligned (4))) ;  // Pal : Town_Pal
extern const unsigned char Town_Tiles[114560] __attribute__ ((aligned (4))) ;  // Pal : Town_Pal

extern const int Large_Info[3]; // BgMode, Width, Height
extern const unsigned short Large_Map[16384] __attribute__ ((aligned (4))) ;  // Pal : Large_Pal
extern const unsigned char Large_Tiles[15552] __attribute__ ((aligned (4))) ;  // Pal : Large_Pal


// Palette files : 
extern const unsigned short Town_Pal[68] __attribute__ ((aligned (4))) ;
extern const unsigned short Large_Pal[28] __attribute__ ((aligned (4))) ;


#endif

