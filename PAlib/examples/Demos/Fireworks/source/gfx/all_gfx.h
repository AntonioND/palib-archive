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
extern const unsigned char red_Texture[64] __attribute__ ((aligned (4))) ;  // Pal : fw_Pal
extern const unsigned char orange_Texture[64] __attribute__ ((aligned (4))) ;  // Pal : fw_Pal
extern const unsigned char yellow_Texture[64] __attribute__ ((aligned (4))) ;  // Pal : fw_Pal
extern const unsigned char green_Texture[64] __attribute__ ((aligned (4))) ;  // Pal : fw_Pal
extern const unsigned char blue_Texture[16] __attribute__ ((aligned (4))) ;  // Pal : fw_Pal
extern const unsigned char indigo_Texture[64] __attribute__ ((aligned (4))) ;  // Pal : fw_Pal
extern const unsigned char violet_Texture[64] __attribute__ ((aligned (4))) ;  // Pal : fw_Pal

// Palette files : 
extern const unsigned short fw_Pal[43] __attribute__ ((aligned (4))) ;


#endif

