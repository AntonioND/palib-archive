#include <PA_BgStruct.h>

extern const char newfont_Tiles[];
extern const char newfont_Map[];
extern const char newfont_Pal[];

const PA_BgStruct newfont = {
	PA_BgNormal,
	256, 192,

	newfont_Tiles,
	newfont_Map,
	{newfont_Pal},

	4096,
	{1536}
};
