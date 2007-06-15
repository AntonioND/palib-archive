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
extern const int Town_Info[3]; // BgMode, Width, Height
extern const unsigned int Town_Map[16384] __attribute__ ((aligned (4))) ;  // Pal : Town_Pal
extern const unsigned char Town_Tiles[114560] __attribute__ ((aligned (4))) ;  // Pal : Town_Pal
extern PAGfx_struct Town; // background pointer


// Palette files : 
extern const unsigned short Town_Pal[68] __attribute__ ((aligned (4))) ;


#endif

