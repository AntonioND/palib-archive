#include <PA9.h>

#include "gif/dev2gif.c"
#include "gif/dgif_lib.c"
#include "gif/egif_lib.c"
#include "gif/gif_err.c"
#include "gif/gif_font.c"
#include "gif/gifalloc.c"
#include "gif/quantize.c"


s16 gifwidth, gifheight; 
u16 *temppalette; // Utilisé pour stocker la palette des gifs...
GifFileType* gifinfo;


const short InterlacedOffset[] = { 0, 4, 2, 1 }; /* The way Interlaced image should. */
const short InterlacedJumps[] = { 8, 8, 4, 2 };    /* be read - offsets and jumps... */

int m;

u8 *tempscreen; // Ecran temporaire...


void PA_Load16bitGif(bool screen, s16 x, s16 y, void *gif){

	gifinfo = (GifFileType*)gif;
	
	u16 temppal[256]; // Palette pour le 16 bits...	
	tempscreen = (u8*)malloc(256*192); // Ecran temporaire

	DecodeGif(gif, (void*)tempscreen, temppal, &gifwidth, &gifheight);
	//u16 width = gifinfo-> Image.Width;
	//u16 height = gifinfo-> Image.Height;
	u16 tempx, tempy, maxx, maxy; // On va copier le gif...
	//u32 i = 0;
	maxx = gifwidth + x;  maxy = gifheight + y;  
	u8 *gfx = tempscreen;
	s16 ytemp = 0;
	
	for(tempy = y; tempy < maxy; tempy++){
		gfx = (u8*)(tempscreen + ytemp);
		for(tempx = x; tempx < maxx; tempx++){
			PA_Put16bitPixel(screen, tempx, tempy, temppal[*gfx]);
			gfx++;
		}
		ytemp+=256; // Ligne suivante
	}
	
	free(tempscreen);
}



int readFunc(GifFileType* GifFile, GifByteType* buf, int count)
{
    char* ptr = (char*)GifFile->UserData;
    memcpy(buf, ptr, count);
    GifFile->UserData = ptr + count;
    return count;
}

void CopyLine(void* dst, void* src, int count)
{
    do
    {
	*(short*) dst = *(short*) src;
	src = (u8*)src + 2;
	dst = (u8*)dst + 2;
	count -= 2;
    }
    while (count >= 0);
}

int DGifGetLineByte(GifFileType *GifFile, GifPixelType *Line, int LineLen)
{
    GifPixelType LineBuf[256];
    CopyLine(LineBuf, Line, LineLen);
    int result = DGifGetLine(GifFile, LineBuf, LineLen);
    CopyLine(Line, LineBuf, LineLen);
    return result;
}


#define GAMMA(x)	(x)

#ifdef _NO_FILEIO
#define PrintGifError()
#endif

#define EXIT_FAILURE 1


/******************************************************************************
* Interpret the command line and scan the given GIF file.		      *
******************************************************************************/

int DecodeGif(const u8 *userData, u8 ScreenBuff[192][256], u16* Palette, s16 *WidthP, s16 *HeightP)
{

    int	i, j, Row, Col, Width, Height, ExtCode, Count;
    GifRecordType RecordType;
    GifByteType *Extension;
    GifFileType *GifFile;
    ColorMapObject *ColorMap;
    
    if ((GifFile = DGifOpen((void*)userData, readFunc)) == NULL) {
	PrintGifError();
	return EXIT_FAILURE;
    }
    
    for (i = 0; i < GifFile->SWidth; i++)  /* Set its color to BackGround. */
		ScreenBuff[0][i] = GifFile->SBackGroundColor;
    for (i = 1; i < GifFile->SHeight; i++) {
		memcpy(ScreenBuff[i], ScreenBuff[0], GifFile->SWidth);
    }
    
    /* Scan the content of the GIF file and load the image(s) in: */
    do {
	if (DGifGetRecordType(GifFile, &RecordType) == GIF_ERROR) {
	    PrintGifError();
	    return EXIT_FAILURE;
	}
	switch (RecordType) {
	case IMAGE_DESC_RECORD_TYPE:
	    if (DGifGetImageDesc(GifFile) == GIF_ERROR) {
		PrintGifError();
		return EXIT_FAILURE;
	    }
	    Row = GifFile->Image.Top; /* Image Position relative to Screen. */
	    Col = GifFile->Image.Left;
	    *WidthP = Width = GifFile->Image.Width;
	    *HeightP = Height = GifFile->Image.Height;
	    for(m=0;m<6;m++) PA_WaitForVBL(); /* WaitForVblank pour que les gifs animés marchent, encore à finioler ^^ */
	    
	    // Update Color map
	    ColorMap = (GifFile->Image.ColorMap
		? GifFile->Image.ColorMap
		: GifFile->SColorMap);
	    

	    i = ColorMap->ColorCount;
	    while (--i >= 0)
	    {
		GifColorType* pColor = &ColorMap->Colors[i];
		
		Palette[i] = PA_RGB8(GAMMA(pColor->Red), GAMMA(pColor->Green), GAMMA(pColor->Blue));
	    }
		
	    if (GifFile->Image.Left + GifFile->Image.Width > GifFile->SWidth ||
		GifFile->Image.Top + GifFile->Image.Height > GifFile->SHeight) {
		return EXIT_FAILURE;
	    }
	    if (GifFile->Image.Interlace) {
		/* Need to perform 4 passes on the images: */
		for (Count = i = 0; i < 4; i++)
		    for (j = Row + InterlacedOffset[i]; j < Row + Height;
		    j += InterlacedJumps[i]) {
			if (DGifGetLineByte(GifFile, &ScreenBuff[j][Col],
			    Width) == GIF_ERROR) {
			    PrintGifError();
			    return EXIT_FAILURE;
			}
		    }
	    }
	    else {
		for (i = 0; i < Height; i++) {
		    if (DGifGetLineByte(GifFile, &ScreenBuff[Row++][Col],
			Width) == GIF_ERROR) {
			PrintGifError();
			return EXIT_FAILURE;
		    }
		}
	    }
	    break;
	case EXTENSION_RECORD_TYPE:
	    /* Skip any extension blocks in file: */
	    if (DGifGetExtension(GifFile, &ExtCode, &Extension) == GIF_ERROR) {
		PrintGifError();
		return EXIT_FAILURE;
	    }
	    while (Extension != NULL) {
		if (DGifGetExtensionNext(GifFile, &Extension) == GIF_ERROR) {
		    PrintGifError();
		    return EXIT_FAILURE;
		}
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
    if (DGifCloseFile(GifFile) == GIF_ERROR) {
	PrintGifError();
	return EXIT_FAILURE;
    }
  
  
    return 0;
}

