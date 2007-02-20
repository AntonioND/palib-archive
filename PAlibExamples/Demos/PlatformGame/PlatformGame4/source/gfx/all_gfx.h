//Gfx converted using Mollusk's PAGfx Converter

//This file contains all the .h, for easier inclusion in a project

#ifndef ALL_GFX_H
#define ALL_GFX_H


// Sprite files : 
extern const unsigned char mario_Sprite[3072] __attribute__ ((aligned (4))) ;  // Pal : sprite0_Pal
extern const unsigned char coin_Sprite[64] __attribute__ ((aligned (4))) ;  // Pal : sprite0_Pal

// Background files : 
extern const int mario_world_Info[3]; // BgMode, Width, Height
extern const unsigned short mario_world_Map[3072] __attribute__ ((aligned (4))) ;  // Pal : mario_world_Pal
extern const unsigned char mario_world_Tiles[1024] __attribute__ ((aligned (4))) ;  // Pal : mario_world_Pal

extern const int hills_Info[3]; // BgMode, Width, Height
extern const unsigned short hills_Map[3072] __attribute__ ((aligned (4))) ;  // Pal : hills_Pal
extern const unsigned char hills_Tiles[2496] __attribute__ ((aligned (4))) ;  // Pal : hills_Pal

extern const int back_Info[3]; // BgMode, Width, Height
extern const unsigned short back_Map[2048] __attribute__ ((aligned (4))) ;  // Pal : back_Pal
extern const unsigned char back_Tiles[256] __attribute__ ((aligned (4))) ;  // Pal : back_Pal

extern const int platformcollisionmap2_Info[3]; // BgMode, Width, Height
extern const unsigned short platformcollisionmap2_Map[3072] __attribute__ ((aligned (4))) ;  // Pal : platformcollisionmap2_Pal
extern const unsigned char platformcollisionmap2_Tiles[192] __attribute__ ((aligned (4))) ;  // Pal : platformcollisionmap2_Pal


// Palette files : 
extern const unsigned short sprite0_Pal[59] __attribute__ ((aligned (4))) ;
extern const unsigned short mario_world_Pal[10] __attribute__ ((aligned (4))) ;
extern const unsigned short hills_Pal[14] __attribute__ ((aligned (4))) ;
extern const unsigned short back_Pal[6] __attribute__ ((aligned (4))) ;
extern const unsigned short platformcollisionmap2_Pal[3] __attribute__ ((aligned (4))) ;


#endif

