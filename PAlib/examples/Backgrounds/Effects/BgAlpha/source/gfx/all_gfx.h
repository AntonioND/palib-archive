//Gfx converted using Mollusk's PAGfx Converter

//This file contains all the .h, for easier inclusion in a project

#ifndef ALL_GFX_H
#define ALL_GFX_H

#ifndef PAGfx_struct
    typedef struct{
    void *Map;
    int MapSize;
    void *Tiles;
    int TileSize;
    void *Palette;
    int *Info;
} PAGfx_struct;
#endif


// Background files : 
extern const int bg0_Info[3]; // BgMode, Width, Height
extern const unsigned short bg0_Map[768] __attribute__ ((aligned (4))) ;  // Pal : bg0_Pal
extern const unsigned char bg0_Tiles[3776] __attribute__ ((aligned (4))) ;  // Pal : bg0_Pal
extern PAGfx_struct bg0; // background pointer

extern const int bg1_Info[3]; // BgMode, Width, Height
extern const unsigned short bg1_Map[768] __attribute__ ((aligned (4))) ;  // Pal : bg1_Pal
extern const unsigned char bg1_Tiles[256] __attribute__ ((aligned (4))) ;  // Pal : bg1_Pal
extern PAGfx_struct bg1; // background pointer


// Palette files : 
extern const unsigned short bg0_Pal[6] __attribute__ ((aligned (4))) ;
extern const unsigned short bg1_Pal[2] __attribute__ ((aligned (4))) ;


#endif

