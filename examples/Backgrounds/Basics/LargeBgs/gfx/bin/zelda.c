#include <PA_BgStruct.h>

extern const char zelda_Tiles[];
extern const char zelda_Map[];
extern const char zelda_Pal[];

const PA_BgStruct zelda = {
	PA_BgLarge,
	2048, 2048,

	zelda_Tiles,
	zelda_Map,
	{zelda_Pal},

	15552,
	{131072}
};
