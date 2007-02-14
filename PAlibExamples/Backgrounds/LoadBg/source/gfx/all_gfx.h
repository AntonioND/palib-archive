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
extern const unsigned char bg0_Tiles[12352] __attribute__ ((aligned (4))) ;  // Pal : bg0_Pal
extern PAGfx_struct bg0; // background pointer


// Palette files : 
extern const unsigned short bg0_Pal[251] __attribute__ ((aligned (4))) ;


#endif

