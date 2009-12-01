#include <PA_BgStruct.h>

extern const char PA_BmpFont2_Tiles[];
extern const char PA_BmpFont2_Map[];
extern const char PA_BmpFont2_Sizes[];

const PA_BgStruct PA_BmpFont2 = {
	PA_Font1bit,
	512, 128,

	PA_BmpFont2_Tiles,
	PA_BmpFont2_Map,
	{PA_BmpFont2_Sizes},

	7360,
	{9}
};
