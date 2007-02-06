#ifdef __cplusplus
extern "C" {
#endif

#include "PA9.h"



PA_GBFS_infos PA_GBFSfile[200]; 
s16 PA_GBFS_nfiles;
GBFS_FILE const* PA_GBFS_FILE;


u32 PA_InitGBFS(void){
s16 dot = 0;
s16 i = 0;

	 REG_EXEMEMCNT &= ~0x80;
 
	PA_GBFS_FILE = find_first_gbfs_file((void*)0x08000000);
	PA_GBFS_nfiles = 0;
	PA_GBFSfile[PA_GBFS_nfiles].File = (void*)gbfs_get_nth_obj(PA_GBFS_FILE, PA_GBFS_nfiles, (char*)PA_GBFSfile[PA_GBFS_nfiles].Name, &PA_GBFSfile[PA_GBFS_nfiles].Length);

while(PA_GBFSfile[PA_GBFS_nfiles].File != NULL){

	// On cherche la fin du nom
	while(PA_GBFSfile[PA_GBFS_nfiles].Name[i]) i++;
	// On en déduit le dernier point, pour avoir l'extension
	while(PA_GBFSfile[PA_GBFS_nfiles].Name[i] != '.') i--; // On cherche l'extension
	dot = i+1;
	
	for (i = 0; PA_GBFSfile[PA_GBFS_nfiles].Name[i+dot] ; i++){
		PA_GBFSfile[PA_GBFS_nfiles].Ext[i] = PA_GBFSfile[PA_GBFS_nfiles].Name[i+dot];
		if (('A' <= PA_GBFSfile[PA_GBFS_nfiles].Ext[i])&&(PA_GBFSfile[PA_GBFS_nfiles].Ext[i] <= 'Z')) PA_GBFSfile[PA_GBFS_nfiles].Ext[i] += 'a' - 'A'; // On passe en minuscule
		PA_GBFSfile[PA_GBFS_nfiles].Ext[i+1] = 0;
		PA_GBFSfile[PA_GBFS_nfiles].Name[i+dot-1] = 0;
		if (PA_GBFSfile[PA_GBFS_nfiles].Name[i+dot-2] == '.') PA_GBFSfile[PA_GBFS_nfiles].Name[i+dot-2] = 0;
	}
	//PA_GBFSfile[PA_GBFS_nfiles].Name[dot] = 0;
	//PA_GBFSfile[PA_GBFS_nfiles].Ext[dot] = 0;
	
	++PA_GBFS_nfiles; // On ajoute un objet...
	PA_GBFSfile[PA_GBFS_nfiles].File = (void*)gbfs_get_nth_obj(PA_GBFS_FILE, PA_GBFS_nfiles, (char*)PA_GBFSfile[PA_GBFS_nfiles].Name, &PA_GBFSfile[PA_GBFS_nfiles].Length);
}

return PA_GBFS_nfiles;
}


s16 PA_GBFSSearchExt(s16 *array, char *extension){
	s16 temp = 0;
	s16 tempvalue = 0;
	u8 loop = 1;
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


#ifdef __cplusplus
}
#endif

