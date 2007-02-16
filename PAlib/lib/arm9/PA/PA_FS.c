#ifdef __cplusplus
extern "C" {
#endif


#include "PA9.h"


PAFSSystems *PA_FSSys;
PAFSFiles *PA_FSFile; // Pointeur sur les infos fichiers...
PAFSFolders *PA_FSFolder; 
char *PAFSStart = (char*)PA_GBA_CART;



void PA_FSAllInit(void){
	while (!((PAFSStart[0] == 'P') && (PAFSStart[1] == 'A') && (PAFSStart[2] == 'F') && (PAFSStart[3] == 'S') && (PAFSStart[4] == '0') && (PAFSStart[5] == '0'))) PAFSStart+=256;

	PAFSStart += 8;
	PA_FSSys = (PAFSSystems*)PAFSStart;

	while (!((PAFSStart[0] == 'D') && (PAFSStart[1] == 'I') && (PAFSStart[2] == 'R') && (PAFSStart[3] == '0') && (PAFSStart[4] == '0') && (PAFSStart[5] == '0'))) PAFSStart++;
	
	PAFSStart += 8;
	PA_FSFolder = (PAFSFolders*)PAFSStart;
	
	while (!((PAFSStart[0] == 'F') && (PAFSStart[1] == 'I') && (PAFSStart[2] == 'L') && (PAFSStart[3] == '0') && (PAFSStart[4] == '0') && (PAFSStart[5] == '0'))) PAFSStart++;
	
	PAFSStart+=8;
	PA_FSFile = (PAFSFiles*)PAFSStart;
	
	while (!((PAFSStart[0] == 'B') && (PAFSStart[1] == 'I') && (PAFSStart[2] == 'N') && (PAFSStart[3] == '0') && (PAFSStart[4] == '0') && (PAFSStart[5] == '0'))) PAFSStart++;

	PAFSStart+=8;
}






s16 PA_FSSearchExt(s16 *array, char *extension){
	s16 temp = 0;
	s16 tempvalue = 0;
	u8 loop = 1;
	while(loop){
		tempvalue = PA_FSGetFile(tempvalue, "", extension);
		array[temp] = tempvalue;
		if (tempvalue == -1) loop = 0;
		tempvalue++;
		temp++;
	}
temp -= 1;
return temp;
}




void PA_FSBgNameLoad(u8 screen, u8 bg_number, char* bg_name){
	char completename[32];
	u8 i;
	for (i= 0; bg_name[i] != 0; i++) completename[i] = bg_name[i];
	completename[i] = '_'; completename[i+1] = 'I'; completename[i+2] = 'n'; completename[i+3] = 'f'; completename[i+4] = 'o';  completename[i+5] = 0;
	PA_FSBgLoad(screen, bg_number, PA_FSGetFile(0, completename, "bin"));
//		PA_OutputText(1, 0, 4+screen, completename);
}






#ifdef __cplusplus
}
#endif





