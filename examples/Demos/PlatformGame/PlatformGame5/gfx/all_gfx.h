// Graphics converted using PAGfx by Mollusk.

#pragma once

#include <PA_BgStruct.h>

#ifdef __cplusplus
extern "C"{
#endif

// Sprites:
extern const unsigned char mario_Sprite[3072] _GFX_ALIGN; // Palette: sprite0_Pal
extern const unsigned char coin_Sprite[64] _GFX_ALIGN; // Palette: sprite0_Pal

// Backgrounds:
extern const PA_BgStruct mario_world;
extern const PA_BgStruct hills;
extern const PA_BgStruct back;
extern const PA_BgStruct platformcollisionmap3;

// Palettes:
extern const unsigned short sprite0_Pal[256] _GFX_ALIGN;

#ifdef __cplusplus
}
#endif
