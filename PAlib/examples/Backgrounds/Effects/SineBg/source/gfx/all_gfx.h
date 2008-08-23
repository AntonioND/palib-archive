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
extern const int zelda_Info[3]; // BgMode, Width, Height
extern const unsigned short zelda_Map[65536] __attribute__ ((aligned (4))) ;  // Pal : zelda_Pal
extern const unsigned char zelda_Tiles[15552] __attribute__ ((aligned (4))) ;  // Pal : zelda_Pal
extern PAGfx_struct zelda; // background pointer


// Palette files : 
extern const unsigned short zelda_Pal[28] __attribute__ ((aligned (4))) ;


#endif

