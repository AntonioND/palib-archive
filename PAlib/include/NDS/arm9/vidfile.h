

int jpgcorrect(u8* video,int num, int *size);


int PA_LoadVid(u8* video,int *taille, int fps,int depart);


int PA_LoadVidGBFS(s8 file,int *taille, int fps,int depart);


void PA_LoadMultiVid(int *taille, int fps,int numvid, ...);


void PA_GBFSsizes(s8 file, int start1, int *end1);


u8 PA_InitGBFSVid();


int PA_LoadMultiVidGBFS(s8 file);


char *correctname(char *name1);
