#include <PA_BgStruct.h>

extern const char pasplash_Tiles[];
extern const char pasplash_Map[];
extern const char pasplash_Pal[];

const PA_BgStruct pasplash = {
	PA_BgNormal,
	256, 192,

	pasplash_Tiles,
	pasplash_Map,
	{pasplash_Pal},

	14528,
	{1536}
};
