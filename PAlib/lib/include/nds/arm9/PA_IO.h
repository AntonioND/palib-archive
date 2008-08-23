#include <stdio.h>
#define read_binary_mode "rb"
#define write_binary_mode "w"

bool PA_InitFat();
long PA_GetFileSize(FILE* pFile);
char* PA_ReadTextFile(char* pathname);
int PA_WriteTextFile(char* pathname, char* text);
