#include <PA_BgStruct.h>

extern const char bg2_Tiles[];
extern const char bg2_Map[];
extern const char bg2_Pal[];

const PA_BgStruct bg2 = {
	PA_BgNormal,
	256, 192,

	bg2_Tiles,
	bg2_Map,
	{bg2_Pal},

	11968,
	{1536}
};
