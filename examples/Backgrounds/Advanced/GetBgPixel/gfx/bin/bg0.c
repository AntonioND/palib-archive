#include <PA_BgStruct.h>

extern const char bg0_Tiles[];
extern const char bg0_Map[];
extern const char bg0_Pal[];

const PA_BgStruct bg0 = {
	PA_BgNormal,
	256, 192,

	bg0_Tiles,
	bg0_Map,
	{bg0_Pal},

	12352,
	{1536}
};
