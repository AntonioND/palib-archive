#include <PA9.h>
#include <sys/dir.h>
#include <stdio.h>
#include <fat.h>


#ifdef __cplusplus
extern "C" {
#endif

void PA_SetVideoMode(u8 screen, u8 mode) {

	// NEW version
	if(screen){
		REG_DISPCNT_SUB &= ~7;   // clear mode flags
		REG_DISPCNT_SUB |= mode; // set mode
	}else{
		REG_DISPCNT &= ~7;   // clear mode flags
		REG_DISPCNT |= mode; // set mode
	}

}

bool PA_Locate(char *start, char *target, bool isDir, int depth, char *result)
{
   struct stat st;
   DIR_ITER *dir = diropen(start);
   static char child[256];
   char temp[256];
   
   if (dir)
   {
      while (dirnext(dir, child, &st) == 0)
      {
         if (strlen(child) == 1 && child[0] == '.')
            continue;
                  
         if (strlen(child) == 2 && child[0] == '.' && child[1] == '.')
            continue;
         
         if (((st.st_mode & S_IFDIR) && isDir) || (!(st.st_mode & S_IFDIR) && !isDir) )
         {
            if (strcasecmp(target, child) == 0) // changed from strcmp to strcasecmp
            {
               strcpy(result, start);
               if (start[strlen(start)-1] != '/')
                  strcat(result, "/");

               strcat(result, child);
               if(isDir) // only add trailing slash if we're searching for a directory
                  strcat(result, "/");
            
               dirclose(dir);
               return true;
            }
         }
            
         if ((st.st_mode & S_IFDIR) && depth > 1)
         {
            strcpy(temp, start);
            if (start[strlen(start)-1] != '/')
               strcat(temp, "/");

            strcat(temp, child);
            strcat(temp, "/");
            
            if (PA_Locate(temp, target, isDir, depth-1, result))
            {
               dirclose(dir);
               return true;
            }
         }
      }
   }
   
   dirclose(dir);
   return false;
}

#ifdef __cplusplus
}
#endif
