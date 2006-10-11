
#ifdef __cplusplus
extern "C" {
#endif

#include <PA9.h>
/*

#include "gif/dgif_lib.c"

#include "gif/gif_err.c"
#include "gif/gif_font.c"
#include "gif/gifalloc.c"
#include "gif/quantize.c"
*/

s16 gifwidth, gifheight; 
u16 *temppalette; // Utilisé pour stocker la palette des gifs...
GifFileType* gifinfo;
u8 *tempscreen; // Ecran temporaire...
extern u8 PA_nBit[2]; 

u8 GifBits; // Mode 8 ou 16 bits

const short InterlacedOffset[] = { 0, 4, 2, 1 }; /* The way Interlaced image should. */
const short InterlacedJumps[] = { 8, 8, 4, 2 };    /* be read - offsets and jumps... */

int m;

   GifPixelType LineBuf[1025]; // Buffer temporaire


//s32 PA_GifSpeed = 6;


int readFunc(GifFileType* GifFile, GifByteType* buf, int count)
{
    char* ptr = (char*)GifFile->UserData;
    memcpy(buf, ptr, count);
    GifFile->UserData = ptr + count;
    return count;
}

void CopyLine(void* dst, void* src, int count){ // Pour 8 bit
/*s16 temp = (count +1) >> 1;
	DMA_Copy(src, dst, temp, DMA_16NOW); // Copy rapide*/

    do {
	*(u16*) dst = *(u16*) src;
	src = (u8*)src + 2;
	dst = (u8*)dst + 2;
	count -= 2;
    }
    while (count > 0); // On fait 4 par 4, puis 2 par 2...
//	if (count >= 0) *(u16*) dst = *(u16*) src; // On finit les 2 derniers
}

void CopyLine2(void* dst, void* src, int count){ // Pour 16 bit
u8 *temp = (u8*)src;
    do {
	*(u16*)dst = temppalette[*temp]; // On prend la couleur de la palette
	temp++;
	dst = (u8*)dst + 2;
	count -= 1;
    }
    while (count > 0);
}



int DGifGetLineByte(GifFileType *GifFile, GifPixelType *Line, int LineLen){
    if (GifBits == 0) CopyLine(LineBuf, Line, LineLen); // On fait un backup 
    int result = DGifGetLine(GifFile, LineBuf, LineLen); // Nouvelle ligne
	if (PA_GifInfo.CurrentFrame >=  PA_GifInfo.StartFrame) {
		if (GifBits == 0) CopyLine(Line, LineBuf, LineLen); // Copie 8 bit
		if (GifBits == 1) CopyLine2(Line, LineBuf, LineLen); // Copie 8 bit	
	}
    return result;
}



#define GAMMA(x)	(x)

/*#ifdef _NO_FILEIO
#define PrintGifError()
#endif*/

//#define EXIT_FAILURE 1


/******************************************************************************
* Interpret the command line and scan the given GIF file.		      *
******************************************************************************/

int DecodeGif(const u8 *userData, u8 *ScreenBuff, u16* Palette, u8 nBits, s16 SWidth)
{

    int	i, j, Row, Col, Width, Height, ExtCode, Count;
    GifRecordType RecordType;
    GifByteType *Extension;
    GifFileType *GifFile;
    ColorMapObject *ColorMap;
	
	PA_GifAnimSpeed(1); // normal speed when starting out...
	PA_GifAnimPlay();
	PA_GifInfo.CurrentFrame = 0;
	
	GifBits = nBits;
	if (GifBits == 1) { // On utilise une palette temporaire...
		temppalette = (u16*)malloc(512); // Ecran temporaire	
		Palette = temppalette;
    }
	
	GifFile = DGifOpen((void*)userData, readFunc);

    
    /* Scan the content of the GIF file and load the image(s) in: */
    do { // Je vire les messages d'erreur pour gagner du temps
	DGifGetRecordType(GifFile, &RecordType);

	switch (RecordType) {
	case IMAGE_DESC_RECORD_TYPE:
		DGifGetImageDesc(GifFile);
	    /*if (DGifGetImageDesc(GifFile) == GIF_ERROR) {
		PrintGifError();
		return EXIT_FAILURE;
	    }*/
	    Row = GifFile->Image.Top; /* Image Position relative to Screen. */
	    Col = GifFile->Image.Left;
	    Width = GifFile->Image.Width;
	    Height = GifFile->Image.Height;
		
//	    for(m=0;m<PA_GifSpeed;m++) PA_WaitForVBL(); /* WaitForVblank pour que les gifs animés marchent, encore à finioler ^^ */
	    
	    // Update Color map
	    ColorMap = (GifFile->Image.ColorMap	? GifFile->Image.ColorMap: GifFile->SColorMap);
		if (PA_GifInfo.CurrentFrame >=  PA_GifInfo.StartFrame) {// Load Palette only if correct frame
			i = ColorMap->ColorCount;
			while (--i >= 0)  {
				GifColorType* pColor = &ColorMap->Colors[i];
				Palette[i] = PA_RGB8(GAMMA(pColor->Red), GAMMA(pColor->Green), GAMMA(pColor->Blue));
			}
		}
		//GifFile->Image.Delay;
		/*
	    if (GifFile->Image.Left + GifFile->Image.Width > GifFile->SWidth ||
		GifFile->Image.Top + GifFile->Image.Height > GifFile->SHeight) {
		return EXIT_FAILURE;
	    }*/
		
		if (GifFile->Image.Interlace) {
		/* Need to perform 4 passes on the images: */
			for (Count = i = 0; i < 4; i++) {
				for (j = Row + InterlacedOffset[i]; j < Row + Height;
					j += InterlacedJumps[i]) {
					DGifGetLineByte(GifFile, (ScreenBuff + (((SWidth*j) + Col) << GifBits)),Width);
				/*if (DGifGetLineByte(GifFile, &ScreenBuff[j][Col],
					Width) == GIF_ERROR) {
					PrintGifError();
					return EXIT_FAILURE;
				}*/
				}
			}
		}
		else {
			for (i = 0; i < Height; i++) {
				DGifGetLineByte(GifFile, (ScreenBuff + (((SWidth*Row) + Col) << GifBits)),Width);
				Row++;
				/*
				if (DGifGetLineByte(GifFile, &ScreenBuff[Row++][Col],
				Width) == GIF_ERROR) {
					PrintGifError();
					return EXIT_FAILURE;
				}*/
			}
		}
	    break;
		
	case EXTENSION_RECORD_TYPE:
	    /* Skip any extension blocks in file: */
		DGifGetExtension(GifFile, &ExtCode, &Extension);

		
//	s32	test = DGifGetExtension(GifFile, &ExtCode, &Extension);
		
		// CHECK HERE FOR GIF TIMING !!
		
	    /*if (DGifGetExtension(GifFile, &ExtCode, &Extension) == GIF_ERROR) {
		PrintGifError();
		return EXIT_FAILURE;
	    }*/
	    while (Extension != NULL) {
			if(ExtCode == GRAPHICS_EXT_FUNC_CODE)
					{
						s32 disposalMethod = (Extension[1] >> 2) & 7;
						disposalMethod = 0;
//						if((Extension[1] & 1) == 1) // transparency color exists
//							pd->transColor = Extension[4];		
 
						int tmpDelay = 0;
						double tmpNumber = 0;
 
						tmpDelay = Extension[2];
						tmpDelay += Extension[3] << 8;
 
						tmpNumber = (double)(tmpDelay) * (double)60;
						tmpNumber = tmpNumber / (double)100;
 
						tmpDelay = (int)tmpNumber;
						s32 time;
						
						PA_GifInfo.CurrentFrame++;
						if (PA_GifInfo.CurrentFrame > PA_GifInfo.EndFrame) PA_GifInfo.Play = 0; // Last frame...
						
						if (PA_GifInfo.CurrentFrame-1 >=  PA_GifInfo.StartFrame){	// Don't wait if not at the right frame yet				
							for(time=0;time*PA_GifInfo.Speed<tmpNumber;time++) {
								PA_WaitForVBL(); 
								while(PA_GifInfo.Play == 2) PA_WaitForVBL(); // Pause animation !
								if (PA_GifInfo.Play == 0) goto GifStop;
							}
						}
						
						
//						pd->aniDelay = tmpDelay;
						
						
					}
					DGifGetExtensionNext(GifFile, &Extension);
					/*if(DGifGetExtensionNext(pd->streamFile, &Extension) == GIF_ERROR) 
					{
						closeGifStreaming(pd);
						pd->error = true;
						sendCallBack(pd, FINISH_PROGRESS, 0);
						return;
					}*/
	    }
	    break;
	case TERMINATE_RECORD_TYPE:
	    break;
	default:		    /* Should be traps by DGifGetRecordType. */
	    break;
	}
    }
    while (RecordType != TERMINATE_RECORD_TYPE);
    
    /* Close file when done */
	GifStop:
	DGifCloseFile(GifFile);
    /*if (DGifCloseFile(GifFile) == GIF_ERROR) {
	PrintGifError();
	return EXIT_FAILURE;
    }*/
	
	if (GifBits == 1) free(temppalette); // On vire la mémoire allouée
	
    return 0;
}


/*
u16 palette[256];
u8 tiles[2][256*8*8];
u8 tempgif[128*128];
u8 tileexists[2][128*128];


u16 spritegfx[8*8*32]; // temp sprite gfx table
u16 spritegif[32*64]; // gif decoding part...

u16 spritepal[256]; // sprite decoding palette

*/




u8* PA_GifToTiles(void *gif, u16 *temppal){

u8 zero = 0;
u16 width = PA_GetGifWidth(gif);
u16 height = PA_GetGifHeight(gif);

u8 *decodgif = (u8*)malloc(width*height);
u8 *newtiles = (u8*)malloc(width*height);

DMA_Force(zero, decodgif, (width*height)>>1, DMA_16NOW); // Blank out the gif
DecodeGif((const u8*)gif, decodgif, temppal, 0, width);

s32 i, j, tile;
tile = 0; 
s32 tempx, tempy;
tempx = 0; tempy = 0;
u16 temp;
/*u8 pal0 = decodgif[2];

// Invert pal0 color...
temp = temppal[pal0];
temppal[pal0] = temppal[0];
temppal[0] = temp;*/

u16 tilemax = (width*height)>>6;
/*for (j = 0; j < height; j++){
	if (decodgif[(j*width)] == 0) decodgif[(j*width)] = pal0;
}*/

for (tile = 0; tile < tilemax; tile++){
	//tileexists[0][tile] = 0;
	for (i = 0; i < 8; i++){ // put the right transp color	
		for (j = 0; j < 8; j++) {
			temp = decodgif[tempx + i + ((tempy+j)*width)];
			/*if (temp == 0) temp = pal0;
			else if (temp == pal0) temp = 0;*/
			newtiles[(tile<<6) + i + (j<<3)] = temp;
			//tileexists[0][tile] |= temp;
		}
	}
	tempx += 8; 
	if (tempx >= width) {
		tempy += 8;
		tempx = 0;
	}
}

free(decodgif); // free the malloc


return (newtiles);

}








/*
PA_GifToTiles((void*)mario_old, (u8*)spritegfx, spritepal, 64, 64);
PA_LoadSpritePal(0, 0, (void*)spritepal);

PA_CreateSprite(0, 0, (void*)spritegfx, OBJ_SIZE_64X64, 1, 0, 128, 64);
PA_SetSpritePrio(0, 0, 3);
PA_SetSpriteDblsize(0, 0, 1);
PA_SetSpriteRotEnable(0,0,0);
PA_SetRotset(0, 0, 0, 128, 128);

//Background
PA_LoadGif(0, skinALL[skinnumber].back);

//Tiles
PA_GifToTiles(skinALL[skinnumber].gif, (u8*)tiles[0], palette, 128, 128);
PA_LoadBg(0, 2, tiles[0], 256*8*8, Blank, BG_256X256, 0, 1);
PA_LoadBgPal(0, 2, (void*)palette);
*/














#ifdef __cplusplus
}
#endif


