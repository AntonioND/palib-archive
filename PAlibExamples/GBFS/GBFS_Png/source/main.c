// Include any BMP file with GBFS, and output it on a 16bit screen !

// Includes
#include <PA9.h>       // Include for PA_Lib


#include "PA_Png.h"

// Function: main()
int main(int argc, char ** argv){

	PA_Init();    // PA Init...
	PA_InitVBL();	// VBL Init...
	
	PA_Init16bitBg(0, 3); 	// Init a 16 bit Bg on screen 0
	PA_Init16bitBg(1, 3); 	// Init a 16 bit Bg on screen 0	
	PA_InitText(1, 0);  // Initialise the text system on the top screen

	u8 nfiles = PA_InitGBFS(); // Init GBFS and get the number of files...
	PA_OutputText(1, 0, 15, "Files : %d", nfiles); // Show the number of files we added
	
	
	unsigned long PNGfileSize = PA_GBFSfile[0].Length; // taille du fichier
	void *pointerToPNG = PA_GBFSfile[0].File;
	
	
	char *pngUncompressedData = NULL;         /* Pointeur vers l'image, initialisé à NULL afin que read_png alloue l'espace nécessaire. */
   png_info *pngInfos;         /* Pointeur vers une structure png_info */

   int width, height, channels,  x, y;
        
   if(read_png((png_byte **)pngUncompressedData, &pngInfos, pointerToPNG, PNGfileSize, 0) != 0)     /* On lit le PNG ! */
   {
      PA_OutputText(1, 0, 18, "Erreur sur le Png...");
   }
	
	width = pngInfos->width;      /* On récupère des informations importantes ... */
   height = pngInfos->height;
   channels = pngInfos->channels;
	
	   if(!PNG_IS_PALETTE(pngInfos->color_type))    /* Si ce n'est pas un PNG palette */
   {
		u16 r, g, b;
      for(y = 0; y < height; y++)
      {
         for(x = 0; x < width; x++)
         {
            char *pixel = GETPIXEL24(pngUncompressedData, x,y,width, channels);
			r = (*GETRED24(pixel)) >> 3;
			g = (*GETGREEN24(pixel))>>3;
			b = (*GETBLUE24(pixel))>>3;
            PA_Put16bitPixel(0, x, y, PA_RGB(r, g, b));
			  /* Affiche le pixel avec une profondeur de 24 bits. */
         }
      }
   }
   else if(PNG_IS_PALETTE(pngInfos->color_type))
   {
      /* L'image ne peut pas être palette car nous n'avons pas passé le flag PNGREAD_WITH_PALETTE à la fonction png_read() */
   }
   else
   {
      PA_OutputText(1, 0, 19, "Type de PNG non supporté !\n");
   }
	
	// output info on the first file...
	PA_OutputText(1, 0, 16, "%s", PA_GBFSfile[0].Name, PA_GetBmpWidth(PA_GBFSfile[0].File), PA_GetBmpHeight(PA_GBFSfile[0].File)); // Name and size...
	// output info on the second file...
	//PA_OutputText(1, 0, 17, "%s, %dx%d", PA_GBFSfile[1].Name, PA_GetBmpWidth(PA_GBFSfile[1].File), PA_GetBmpHeight(PA_GBFSfile[1].File)); // Name and size...

	while(1){	// Infinite loop
		PA_WaitForVBL();
	}
		
	return 0;
} // End of main()















