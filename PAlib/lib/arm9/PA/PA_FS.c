#include "PA9.h"


PAFSSystems *PA_FSSys;
PAFSFiles *PA_FSFile; // Pointeur sur les infos fichiers...
PAFSFolders *PA_FSFolder; 
char *PAFSStart = (char*)0x08000000;




u32 PA_FSInit(void){
	PAFSStart =  (char*)0x08000000; // On va chercher dans la ROM
	
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
	
	return (PA_FSSys->Nfiles);
}






s16 PA_FSSearchExt(s16 *array, char *extension){
	s16 temp = 0;
	s16 tempvalue = 0;
	bool loop = 1;
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










