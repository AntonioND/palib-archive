#include "PA9.h"



PA_GBFS_infos PA_GBFSfile[200]; 
s16 PA_GBFS_nfiles;
GBFS_FILE const* PA_GBFS_FILE;



s16 PA_GBFSSearchExt(s16 *array, char *extension){
	s16 temp = 0;
	s16 tempvalue = 0;
	bool loop = 1;
	while(loop){
		tempvalue = PA_GetGBFSFile(tempvalue, "", extension);
		array[temp] = tempvalue;
		if (tempvalue == -1) loop = 0;
		tempvalue++;
		temp++;
	}
temp -= 1;
return temp;
}
