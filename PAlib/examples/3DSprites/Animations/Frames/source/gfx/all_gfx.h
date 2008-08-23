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
extern const unsigned char frames_Texture[4096] __attribute__ ((aligned (4))) ;  // Pal : frames_Pal

// Palette files : 
extern const unsigned short frames_Pal[19] __attribute__ ((aligned (4))) ;


#endif

