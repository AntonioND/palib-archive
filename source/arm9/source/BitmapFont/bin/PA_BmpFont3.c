#include <PA_BgStruct.h>

extern const char PA_BmpFont3_Tiles[];
extern const char PA_BmpFont3_Map[];
extern const char PA_BmpFont3_Sizes[];

const PA_BgStruct PA_BmpFont3 = {
	PA_Font1bit,
	512, 128,

	PA_BmpFont3_Tiles,
	PA_BmpFont3_Map,
	{PA_BmpFont3_Sizes},

	12544,
	{12}
};
