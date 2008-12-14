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
extern const unsigned char mollusk_Texture[1024] __attribute__ ((aligned (4))) ;  // Pal : mollusk_Pal
extern const unsigned char AnarX_Texture[1024] __attribute__ ((aligned (4))) ;  // Pal : AnarX_Pal
extern const unsigned char Abrexxes_Texture[1024] __attribute__ ((aligned (4))) ;  // Pal : Abrexxes_Pal

// Palette files : 
extern const unsigned short mollusk_Pal[4] __attribute__ ((aligned (4))) ;
extern const unsigned short AnarX_Pal[4] __attribute__ ((aligned (4))) ;
extern const unsigned short Abrexxes_Pal[4] __attribute__ ((aligned (4))) ;


#endif

