#include <PA_BgStruct.h>

extern const char BG2_Tiles[];
extern const char BG2_Map[];
extern const char BG2_Pal[];

const PA_BgStruct BG2 = {
	PA_BgNormal,
	256, 256,

	BG2_Tiles,
	BG2_Map,
	{BG2_Pal},

	1472,
	{2048}
};
