/* EFS_Lib example - by Noda
   PALib version by Sweater Fish

For your own project, just include and init EFS_Lib as shown in this
example.

Be sure the Makefile you're using is set to USE EFS, too.

EFS_Lib installation instructions:
As EFS_Lib is a separate library, it is not contained in PALib. You can
copy the needed files from this example to your own project.

Place the efs_lib.h/.c files in the "source" directory of your project
with efs.exe and the "efsroot" folder in the project's main directory.
*/




#include <PA9.h>
#include <unistd.h>


#include "efs_lib.h"    // include EFS lib

int main(void) {

    // init PA_Lib
    PA_Init();
    PA_InitVBL();
	
    PA_InitText(1, 0);
    PA_InitText(0, 0);
    
    // init EFSlib & libfat
    if(EFS_Init(EFS_AND_FAT | EFS_DEFAULT_DEVICE, NULL)) {
        PA_OutputSimpleText(1, 0, 0, "EFS init ok");
        PA_OutputText(1, 0, 1, "found NDS path: %s", efs_path);    
        
        DIR_ITER* dir;
        struct stat st;
        s8 nb;
        FILE* file;
        u8* buffer;
        int i, size;

        // open a text file and read its contents
        file = fopen("/test.txt", "rb");
        if(file != NULL) {
            // get file size using stat            
            stat("/test.txt", &st);
            size = st.st_size;
            
            buffer = (u8*)malloc(size);
            fread(buffer, 1, size, file);
            buffer[size-1] = '\0';
            PA_OutputText(1, 0, 4, "/test.txt content: '%s'", buffer);
            PA_OutputText(1, 10, 5, "size: %d bytes", size);
            free(buffer);
            fclose(file); 
        }

        // open another file, read its content
        file = fopen("/folder/test.txt", "rb");
        if(file != NULL) {
            // get file size
            fseek(file, 0, SEEK_END);
            size = ftell(file); 
            fseek(file, 0, SEEK_SET);

            buffer = (u8*)malloc(size);
            fread(buffer, 1, size, file);
            buffer[size-1] = '\0';
            PA_OutputText(1, 0, 7, "/folder/test.txt content:\n%s", buffer);
            free(buffer); 
            fclose(file);
        }
            
        // reopen the file, modify its content
        file = fopen("/folder/test.txt", "rb+");
        if(file != NULL) {
            nb = fwrite("16b Written OK!", 1, 16, file);
            PA_OutputText(1, 0, 10, "write test done! : %d bytes", nb);
            fclose(file);
        }

        // reopen another file, read its content again, 1 byte at a time
        file = fopen("/folder/dummy/.././test.txt", "rb");  // funky path to test relative path parsing
        if(file != NULL) {
            // get file size
            fseek(file, 0, SEEK_END);
            size = ftell(file); 
            fseek(file, 0, SEEK_SET);

            buffer = (u8*)malloc(size);
            
            i = 0;
            while(i < size) {
                fread(&buffer[i], 1, 1, file);
                i++;
            }
            
            buffer[size-1] = '\0';
            PA_OutputText(1, 0, 12, "/folder/test.txt new content:\n%s", buffer);
            free(buffer); 
            fclose(file);
        }

        PA_OutputSimpleText(1, 0, 15, "Press A for directory tests.");
        PA_WaitFor(Pad.Newpress.A);

        // open root directory then list its content
        PA_OutputText(0, 0, 0, "Listing '/' directory:");
        dir = diropen(".");
        
        if(dir != NULL) {
            i = 0;
            buffer = (u8*)malloc(EFS_MAXNAMELEN);

            while(!(dirnext(dir, (char*)buffer, &st))) {
                if(st.st_mode & S_IFDIR)
                    PA_OutputText(0, 0, i++, "DIR : %s", buffer);
                else
                    PA_OutputText(0, 0, i++, "FILE: %s, %d bytes", buffer, st.st_size);
            }
            
            PA_OutputSimpleText(0, 0, i++, "end of directory.");
            PA_OutputSimpleText(0, 0, i++, "directory reset, first file is:");
            
            dirreset(dir);
            dirnext(dir, (char*)buffer, &st);
            if(st.st_mode & S_IFDIR)
                PA_OutputText(0, 0, i++, "DIR : %s", buffer);
            else
                PA_OutputText(0, 0, i++, "FILE: %s, %d bytes", buffer, st.st_size);
            
            dirclose(dir);
            free(buffer);
        }

        // chdir to a directory then list its content
        PA_OutputSimpleText(0, 0, i++, "Listing '/list/' directory:");
        chdir("/list/");
        dir = diropen("./");
        
        if(dir != NULL) {
            buffer = (u8*)malloc(EFS_MAXNAMELEN);
            
            while(!(dirnext(dir, (char*)buffer, &st))) {
                if(st.st_mode & S_IFDIR)
                    PA_OutputText(0, 0, i++, "DIR : %s", buffer);
                else
                    PA_OutputText(0, 0, i++, "FILE: %s, %d bytes", buffer, st.st_size);
            }
            
            PA_OutputSimpleText(0, 0, i++, "end of directory.");
            dirclose(dir);
            free(buffer);
        }

    } else {
        PA_OutputSimpleText(1, 0, 0, "EFS init error!");
    }

    while(1) {
        PA_WaitForVBL();
    }

    return 0;
}
