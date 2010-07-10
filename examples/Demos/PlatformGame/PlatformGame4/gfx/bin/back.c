#include <PA_BgStruct.h>

extern const char back_Tiles[];
extern const char back_Map[];
extern const char back_Pal[];

const PA_BgStruct back = {
	PA_BgNormal,
	512, 256,

	back_Tiles,
	back_Map,
	{back_Pal},

	256,
	{4096}
};
