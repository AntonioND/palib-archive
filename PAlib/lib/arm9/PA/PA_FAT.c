#include <PA9.h>

#ifdef FAT_DRIVER
	#include "../../../FAT/disc_io.c"
	#include "../../../FAT/gba_nds_fat.c"
	#include "../../../FAT/io_fcsr.c"
	#include "../../../FAT/io_m3cf.c"
	#include "../../../FAT/io_mpcf.c"
	#include "../../../FAT/io_nmmc.c"
	#include "../../../FAT/io_sccf.c"
	#include "../../../FAT/io_scsd.c"
#endif
