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


// Sprite files : 
extern const unsigned char pokekun_Sprite[2048] __attribute__ ((aligned (4))) ;  // Pal : pokekun_Pal

// Palette files : 
extern const unsigned short pokekun_Pal[5] __attribute__ ((aligned (4))) ;


#endif

