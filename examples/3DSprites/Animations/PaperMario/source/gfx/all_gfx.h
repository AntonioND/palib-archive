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
extern const int mariobg_Info[3]; // BgMode, Width, Height
extern const unsigned short mariobg_Map[768] __attribute__ ((aligned (4))) ;  // Pal : mariobg_Pal
extern const unsigned char mariobg_Tiles[11840] __attribute__ ((aligned (4))) ;  // Pal : mariobg_Pal
extern PAGfx_struct mariobg; // background pointer


// Sprite files : 
extern const unsigned char mario_Texture[16384] __attribute__ ((aligned (4))) ;  // Pal : mario_Pal

// Palette files : 
extern const unsigned short mariobg_Pal[253] __attribute__ ((aligned (4))) ;
extern const unsigned short mario_Pal[54] __attribute__ ((aligned (4))) ;


#endif

