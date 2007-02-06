#include <PA9.h>
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

bool PA_InitFat() {

	return fatInitDefault();
}


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
  
  pFile = fopen ( "/test.txt" , write_binary_mode );
  PA_WaitForVBL();
  
  if (pFile==NULL) {
 
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
 PA_OutputText(0,0,0,"%s  ", text);

  return bytesTransferred;
	
}

