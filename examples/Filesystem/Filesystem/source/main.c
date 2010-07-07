#include <PA9.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {

    // init PAlib
    PA_Init();
	
    PA_LoadDefaultText(1, 0);
    PA_LoadDefaultText(0, 0);

    if(nitroFSInit()) {
        PA_OutputSimpleText(1, 0, 0, "Filesystem init ok");
        PA_OutputText(1, 0, 1, "NDS path: %s", argv[0]);    

	struct stat st;
        FILE* file;
        u8* buffer;
        int i = 0, size;

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

    } else {
        PA_OutputSimpleText(1, 0, 0, "Filesystem init error!");
    }

    while(1) {
        PA_WaitForVBL();
    }

    return 0;
}