#include <PA9.h>
#include <sys/dir.h>
#include <stdio.h>
#include <fat.h>
#include "arm9/PA_IO.h"

long PA_GetFileSize(FILE* pFile) {
  
  long lSize;
  fseek (pFile , 0 , SEEK_END);
  lSize = ftell (pFile);
  rewind (pFile);

  return lSize;
}

#define PA_InitFat fatInitDefault


char* PA_ReadTextFile(char* pathname) {
  FILE* pFile;
  long lSize;
  char * buffer;
  char* buffer2=" ";

  pFile = fopen ( pathname , read_binary_mode );
  
  if (pFile==NULL) {
  
	return "no File Found! Create one now!";
  }

  // obtain file size.

lSize = PA_GetFileSize(pFile);

  // allocate memory to contain the whole file.
  buffer  = (char*) malloc (lSize);
  
  if (buffer == NULL) return "buffer error";

  // copy the file into the buffer.
  fread (buffer,1,lSize,pFile);

  /*** the whole file is loaded in the buffer. ***/
	strcpy (buffer2,buffer);
  // terminate
  fclose (pFile);
  free (buffer);
  return buffer2;
}


int PA_WriteTextFile(char* pathname, char* text) {

  FILE* pFile;
  u32 bytesTransferred;
  
  pFile = fopen ( pathname , write_binary_mode );
  PA_WaitForVBL();
  
  if (pFile==NULL) {
	return 0;
  }
	
	bytesTransferred=fwrite(text,1,strlen(text),pFile);   //write the data to file and save the amount of bytes written.
			  
			  
	if(bytesTransferred<1) {   //check for success
		
		rewind(pFile);
		fclose (pFile);
		return 0; //no bytes written, error
	}
	
	rewind(pFile);
	fclose (pFile);
		
	PA_WaitForVBL();
	

  // terminate
// PA_OutputText(0,0,0,"%s  ", text);

  return bytesTransferred;
	
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
