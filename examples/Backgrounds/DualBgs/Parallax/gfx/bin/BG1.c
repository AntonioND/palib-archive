#include <PA_BgStruct.h>

extern const char BG1_Tiles[];
extern const char BG1_Map[];
extern const char BG1_Pal[];

const PA_BgStruct BG1 = {
	PA_BgNormal,
	256, 256,

	BG1_Tiles,
	BG1_Map,
	{BG1_Pal},

	3712,
	{2048}
};
