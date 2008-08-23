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
extern const int pasplash_Info[3]; // BgMode, Width, Height
extern const unsigned short pasplash_Map[768] __attribute__ ((aligned (4))) ;  // Pal : pasplash_Pal
extern const unsigned char pasplash_Tiles[14528] __attribute__ ((aligned (4))) ;  // Pal : pasplash_Pal
extern PAGfx_struct pasplash; // background pointer


// Palette files : 
extern const unsigned short pasplash_Pal[256] __attribute__ ((aligned (4))) ;


#endif

