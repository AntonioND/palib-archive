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
extern const int WorldMap_Info[3]; // BgMode, Width, Height
extern const unsigned short WorldMap_Map[65536] __attribute__ ((aligned (4))) ;  // Pal : WorldMap_Pal
extern const unsigned char WorldMap_Tiles[15552] __attribute__ ((aligned (4))) ;  // Pal : WorldMap_Pal
extern PAGfx_struct WorldMap; // background pointer

extern const int smb_Info[3]; // BgMode, Width, Height
extern const unsigned short smb_Map[768] __attribute__ ((aligned (4))) ;  // Pal : smb_Pal
extern const unsigned char smb_Tiles[4224] __attribute__ ((aligned (4))) ;  // Pal : smb_Pal
extern PAGfx_struct smb; // background pointer


// Palette files : 
extern const unsigned short WorldMap_Pal[28] __attribute__ ((aligned (4))) ;
extern const unsigned short smb_Pal[10] __attribute__ ((aligned (4))) ;


#endif

