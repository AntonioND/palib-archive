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
extern const unsigned char harry_Sprite[1024] __attribute__ ((aligned (4))) ;  // Pal : harry_Pal
extern const unsigned char mario_Sprite[256] __attribute__ ((aligned (4))) ;  // Pal : mario_Pal
extern const unsigned char pokekun_Sprite[2048] __attribute__ ((aligned (4))) ;  // Pal : pokekun_Pal
extern const unsigned char sonic_Sprite[1024] __attribute__ ((aligned (4))) ;  // Pal : sonic_Pal

// Palette files : 
extern const unsigned short harry_Pal[5] __attribute__ ((aligned (4))) ;
extern const unsigned short mario_Pal[4] __attribute__ ((aligned (4))) ;
extern const unsigned short pokekun_Pal[5] __attribute__ ((aligned (4))) ;
extern const unsigned short sonic_Pal[14] __attribute__ ((aligned (4))) ;


#endif

