#include <PA_BgStruct.h>

extern const char bg3_Tiles[];
extern const char bg3_Map[];
extern const char bg3_Pal[];

const PA_BgStruct bg3 = {
	PA_BgNormal,
	256, 192,

	bg3_Tiles,
	bg3_Map,
	{bg3_Pal},

	11264,
	{1536}
};
