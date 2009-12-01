#include <PA_BgStruct.h>

extern const char PA_BmpFont4_Tiles[];
extern const char PA_BmpFont4_Map[];
extern const char PA_BmpFont4_Sizes[];

const PA_BgStruct PA_BmpFont4 = {
	PA_Font1bit,
	512, 128,

	PA_BmpFont4_Tiles,
	PA_BmpFont4_Map,
	{PA_BmpFont4_Sizes},

	16768,
	{14}
};
