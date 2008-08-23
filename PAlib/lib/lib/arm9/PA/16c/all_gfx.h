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
#define c16_text0_Height 6
extern const char c16_text0_Sizes[256];
extern const int c16_text0_Info[3]; // BgMode, Width, Height
extern const unsigned short c16_text0_Map[256] __attribute__ ((aligned (4))) ;  // Pal : c16_text0_Pal
extern const unsigned char c16_text0_Tiles[800] __attribute__ ((aligned (4))) ;  // Pal : c16_text0_Pal
extern PAGfx_struct c16_text0; // background pointer

#define c16_text1_Height 8
extern const char c16_text1_Sizes[256];
extern const int c16_text1_Info[3]; // BgMode, Width, Height
extern const unsigned short c16_text1_Map[256] __attribute__ ((aligned (4))) ;  // Pal : c16_text1_Pal
extern const unsigned char c16_text1_Tiles[840] __attribute__ ((aligned (4))) ;  // Pal : c16_text1_Pal
extern PAGfx_struct c16_text1; // background pointer

#define c16_text2_Height 9
extern const char c16_text2_Sizes[256];
extern const int c16_text2_Info[3]; // BgMode, Width, Height
extern const unsigned short c16_text2_Map[1024] __attribute__ ((aligned (4))) ;  // Pal : c16_text2_Pal
extern const unsigned char c16_text2_Tiles[920] __attribute__ ((aligned (4))) ;  // Pal : c16_text2_Pal
extern PAGfx_struct c16_text2; // background pointer

#define c16_text3_Height 12
extern const char c16_text3_Sizes[256];
extern const int c16_text3_Info[3]; // BgMode, Width, Height
extern const unsigned short c16_text3_Map[1024] __attribute__ ((aligned (4))) ;  // Pal : c16_text3_Pal
extern const unsigned char c16_text3_Tiles[1568] __attribute__ ((aligned (4))) ;  // Pal : c16_text3_Pal
extern PAGfx_struct c16_text3; // background pointer

#define c16_text4_Height 14
extern const char c16_text4_Sizes[256];
extern const int c16_text4_Info[3]; // BgMode, Width, Height
extern const unsigned short c16_text4_Map[1024] __attribute__ ((aligned (4))) ;  // Pal : c16_text4_Pal
extern const unsigned char c16_text4_Tiles[2096] __attribute__ ((aligned (4))) ;  // Pal : c16_text4_Pal
extern PAGfx_struct c16_text4; // background pointer


// Palette files : 
extern const unsigned short c16_text0_Pal[2] __attribute__ ((aligned (4))) ;
extern const unsigned short c16_text1_Pal[2] __attribute__ ((aligned (4))) ;
extern const unsigned short c16_text2_Pal[2] __attribute__ ((aligned (4))) ;
extern const unsigned short c16_text3_Pal[2] __attribute__ ((aligned (4))) ;
extern const unsigned short c16_text4_Pal[2] __attribute__ ((aligned (4))) ;


#endif

