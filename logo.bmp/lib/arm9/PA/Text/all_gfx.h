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
extern const int PA_text_Info[3]; // BgMode, Width, Height
extern const unsigned short PA_text_Map[768] __attribute__ ((aligned (4))) ;  // Pal : PA_text_Pal
extern const unsigned char PA_text_Tiles[6080] __attribute__ ((aligned (4))) ;  // Pal : PA_text_Pal
extern PAGfx_struct PA_text; // background pointer

extern const int PA_text2_Info[3]; // BgMode, Width, Height
extern const unsigned short PA_text2_Map[768] __attribute__ ((aligned (4))) ;  // Pal : PA_text2_Pal
extern const unsigned char PA_text2_Tiles[11200] __attribute__ ((aligned (4))) ;  // Pal : PA_text2_Pal
extern PAGfx_struct PA_text2; // background pointer


// Palette files : 
extern const unsigned short PA_text_Pal[5] __attribute__ ((aligned (4))) ;
extern const unsigned short PA_text2_Pal[6] __attribute__ ((aligned (4))) ;


#endif

