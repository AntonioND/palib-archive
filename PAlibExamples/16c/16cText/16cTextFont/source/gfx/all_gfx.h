//Gfx converted using Mollusk's PAGfx Converter

//This file contains all the .h, for easier inclusion in a project

#ifndef ALL_GFX_H
#define ALL_GFX_H


// Background files : 
extern const char bigfont_Height;
extern const char bigfont_Sizes[256];
extern const int bigfont_Info[3]; // BgMode, Width, Height
extern const unsigned short bigfont_Map[1024] __attribute__ ((aligned (4))) ;  // Pal : bigfont_Pal
extern const unsigned char bigfont_Tiles[8256] __attribute__ ((aligned (4))) ;  // Pal : bigfont_Pal

extern const char smallfont_Height;
extern const char smallfont_Sizes[256];
extern const int smallfont_Info[3]; // BgMode, Width, Height
extern const unsigned short smallfont_Map[256] __attribute__ ((aligned (4))) ;  // Pal : smallfont_Pal
extern const unsigned char smallfont_Tiles[3168] __attribute__ ((aligned (4))) ;  // Pal : smallfont_Pal


// Palette files : 
extern const unsigned short bigfont_Pal[2] __attribute__ ((aligned (4))) ;
extern const unsigned short smallfont_Pal[2] __attribute__ ((aligned (4))) ;


#endif

