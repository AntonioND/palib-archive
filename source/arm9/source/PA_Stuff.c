#include <PA9.h>
#include <dirent.h>

void PA_SetVideoMode(u8 screen, u8 mode) {
	// NEW version
	if (screen) {
		REG_DISPCNT_SUB &= ~7;   // clear mode flags
		REG_DISPCNT_SUB |= mode; // set mode
	} else {
		REG_DISPCNT &= ~7;   // clear mode flags
		REG_DISPCNT |= mode; // set mode
	}
}

bool PA_Locate(char *start, char *target, bool isDir, int depth, char *result) { 
	DIR *dir;
	dir = opendir(start); 
	char temp[256];
	struct dirent *pent; 
	struct stat statbuf;

	if (dir) { 
		while ((pent=readdir(dir))!=NULL) { 
			stat(pent->d_name,&statbuf);
			
	    	if(strcmp(".", pent->d_name) == 0 || strcmp("..", pent->d_name) == 0)
                continue;
			
			if ((S_ISDIR(statbuf.st_mode) && isDir) || (!S_ISDIR(statbuf.st_mode) && !isDir)) {
	    		if(strcasecmp(target, pent->d_name) == 0) {
					strcpy(result, start); 
					if (start[strlen(start)-1] != '/') 
						strcat(result, "/"); 
					strcat(result, pent->d_name); 
					if (isDir)
					strcat(result, "/"); 
					closedir(dir); 
					return true; 
				} 
			} 

			if (S_ISDIR(statbuf.st_mode) && depth > 1) {
				strcpy(temp, start); 
				if (start[strlen(start)-1] != '/') 
					strcat(temp, "/"); 
				strcat(temp, pent->d_name); 
				strcat(temp, "/"); 
 
                if (PA_Locate(temp, target, isDir, depth-1, result)) {
					closedir(dir); 
					return true; 
				} 
			} 
		} 
	} 

	closedir(dir);
	return false; 
}
