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
extern const unsigned char souris_Sprite[512] __attribute__ ((aligned (4))) ;  // Pal : palettesouris_Pal
extern const unsigned char curseur_Sprite[128] __attribute__ ((aligned (4))) ;  // Pal : palettecurseur_Pal
extern const unsigned char boutonquitter_Sprite[2048] __attribute__ ((aligned (4))) ;  // Pal : boutonquitter_Pal

// Palette files : 
extern const unsigned short palettesouris_Pal[256] __attribute__ ((aligned (4))) ;
extern const unsigned short palettecurseur_Pal[256] __attribute__ ((aligned (4))) ;
extern const unsigned short boutonquitter_Pal[256] __attribute__ ((aligned (4))) ;


#endif

