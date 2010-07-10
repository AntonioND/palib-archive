#include <PA_BgStruct.h>

extern const char BG3_Tiles[];
extern const char BG3_Map[];
extern const char BG3_Pal[];

const PA_BgStruct BG3 = {
	PA_BgNormal,
	256, 256,

	BG3_Tiles,
	BG3_Map,
	{BG3_Pal},

	65536,
	{2048}
};
