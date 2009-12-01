#include <PA_BgStruct.h>

extern const char PA_TransBg_Tiles[];
extern const char PA_TransBg_Map[];
extern const char PA_TransBg_Pal[];

const PA_BgStruct PA_TransBg = {
	PA_BgNormal,
	256, 192,

	PA_TransBg_Tiles,
	PA_TransBg_Map,
	{PA_TransBg_Pal},

	3584,
	{1536}
};
