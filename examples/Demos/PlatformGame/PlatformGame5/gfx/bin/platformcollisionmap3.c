#include <PA_BgStruct.h>

extern const char platformcollisionmap3_Tiles[];
extern const char platformcollisionmap3_Map[];
extern const char platformcollisionmap3_Pal[];

const PA_BgStruct platformcollisionmap3 = {
	PA_BgLarge,
	1024, 256,

	platformcollisionmap3_Tiles,
	platformcollisionmap3_Map,
	{platformcollisionmap3_Pal},

	256,
	{8192}
};
