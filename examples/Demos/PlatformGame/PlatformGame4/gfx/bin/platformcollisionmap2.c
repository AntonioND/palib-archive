#include <PA_BgStruct.h>

extern const char platformcollisionmap2_Tiles[];
extern const char platformcollisionmap2_Map[];
extern const char platformcollisionmap2_Pal[];

const PA_BgStruct platformcollisionmap2 = {
	PA_BgLarge,
	1024, 256,

	platformcollisionmap2_Tiles,
	platformcollisionmap2_Map,
	{platformcollisionmap2_Pal},

	192,
	{8192}
};
