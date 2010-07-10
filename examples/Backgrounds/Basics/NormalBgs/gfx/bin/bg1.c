#include <PA_BgStruct.h>

extern const char bg1_Tiles[];
extern const char bg1_Map[];
extern const char bg1_Pal[];

const PA_BgStruct bg1 = {
	PA_BgNormal,
	256, 192,

	bg1_Tiles,
	bg1_Map,
	{bg1_Pal},

	11008,
	{1536}
};
