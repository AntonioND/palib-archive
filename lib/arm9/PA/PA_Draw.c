#ifdef __cplusplus
extern "C" {
#endif


#include "PA9.h"


//////////////////////////////////////////////////////////////////////
// Buffer mode
//////////////////////////////////////////////////////////////////////


u16 PA_oldx[2], PA_oldy[2];
u8 PA_drawsize[2];
u16 *PA_DrawBg[2];
u8 PA_nBit[2]; // 8 or 16 on each screen...

u16 PA_temppal[256];



u16 tempvar;



void PA_Draw8bitLineEx(u8 screen, s16 basex, s16 basey, s16 endx, s16 endy, u8 color, s8 size);


void PA_Init8bitBg(u8 screen, u8 bg_priority){

PA_DeleteBg(screen, 3);
PA_nBit[screen] = 0; // 8 bit

	PA_BGXPA(screen, 3) = 1 << 8;
	PA_BGXPB(screen, 3) = 0;
	PA_BGXPC(screen, 3) = 0;
	PA_BGXPD(screen, 3) = 1 << 8;
	PA_BGXX(screen, 3) = 0;
	PA_BGXY(screen, 3) = 0;	

	PA_DrawBg[screen] =  (u16*)(0x06000000 + (0x200000 *  screen) + 320 * 256);
	DMA_Copy(Blank, (void*)PA_DrawBg[screen], 256*96, DMA_16NOW);
		
	//this is just used so we can write red color bits to one frame and green to the 
	//other
	
charsetstart[screen] = 5; // On se réserve la moitié de la mémoire...
charblocks[screen][40] = 1; // Block la mémoire

	_REG16(REG_BGSCREEN(screen)) &= ~7;
	_REG16(REG_BGSCREEN(screen)) |= (0x100 << (3)) | MODE_3_2D;
	_REG16(REG_BGCNT(screen, 3)) = bg_priority | BG_BMP8_256x256 | BG_BMP_BASE(5);
PA_SetDrawSize(screen, 1);
}





void PA_LoadBmpToBuffer(u16 *Buffer, s16 x, s16 y, void *bmp, s16 SWidth){

	u8 *temp = (u8*)bmp;
	BMP_Headers *Bmpinfo = (BMP_Headers*)(temp+14);
	
	u8 *gfx = temp+54;
	u16 *gfx2 = (u16*)(temp+54); // Pour le mode 16 bit...
	s32 r, g, b;  s16 tempx, tempy;
	s16 lx = Bmpinfo->Width;   s16 ly = Bmpinfo->Height;
	u16 Bits = Bmpinfo->BitsperPixel;
	//Buffer = (u16*)(Buffer + ((x + (y*SWidth)) << 1)); // Position de départ
	
	s32 i = 0;
	if (Bits > 16){ // Pour 24 et 32 bits
		for (tempy = ly-1; tempy > -1; tempy--){
			for (tempx = 0; tempx < lx; tempx++){
				b = (gfx[i] >> 3)&31;	i++;
				g = (gfx[i] >> 3)&31;	i++;
				r = (gfx[i] >> 3)&31;	i++;	
				if (Bits == 32) i++; // On passe le bit alpha
				//PA_Put16bitPixel(screen, x + tempx, y + tempy, PA_RGB(r, g, b));
				Buffer[x + tempx + ((y + tempy) * SWidth)] = PA_RGB(r, g, b);
			}
			while(i&3) i++; // Padding....
		}
	}
	if (Bits == 16){
		for (tempy = ly-1; tempy > -1; tempy--){
			for (tempx = 0; tempx < lx; tempx++){
				b = *gfx2&31;
				g = (*gfx2>>5)&31;
				r = (*gfx2>>10)&31;		
				//PA_Put16bitPixel(screen, x + tempx, y + tempy, PA_RGB(r, g, b));
				Buffer[x + tempx + ((y + tempy) * SWidth)] = PA_RGB(r, g, b);
				gfx2++; // On passe au pixel suivant
			}
			s32 temp = (s32)gfx2;
			while(temp&3) temp++; // Padding....
			gfx2 = (u16*)temp;
		}	
	}
}


















void PA_Draw8bitLine(u8 screen, u16 x1, u16 y1, u16 x2, u16 y2, u8 color){
  int i,dx,dy,sdx,sdy,dxabs,dyabs,x,y,px,py;

  dx=x2-x1;      /* the horizontal distance of the line */
  dy=y2-y1;      /* the vertical distance of the line */
  dxabs = dx;
  sdx = 1;
  if (dx < 0) {
	  dxabs = -dx;
	  sdx = -1;
  }
  dyabs = dy;
  sdy = 1;
  if (dy < 0) {
	  dyabs = -dy;
	  sdy = -1;
  }

  x=dyabs>>1;
  y=dxabs>>1;
  px=x1;
  py=y1;
  
PA_Put8bitPixel(screen, px, py, color);


  if (dxabs>=dyabs) {
    for(i=0;i<dxabs;i++)  {
      y+=dyabs;
      if (y>=dxabs)  {
        y-=dxabs;
        py+=sdy;
      }
      px+=sdx;
      PA_Put8bitPixel(screen, px, py, color);
    }
  }
  else {
    for(i=0;i<dyabs;i++) {
      x+=dxabs;
      if (x>=dyabs)  {
        x-=dyabs;
        px+=sdx;
      }
      py+=sdy;
      PA_Put8bitPixel(screen, px, py, color);
    }
  }
}





void PA_Init16bitBg(u8 screen, u8 bg_priority){

PA_DeleteBg(screen, 3);
PA_nBit[screen] = 1; // 16 bit

	PA_BGXPA(screen, 3) = 1 << 8;
	PA_BGXPB(screen, 3) = 0;
	PA_BGXPC(screen, 3) = 0;
	PA_BGXPD(screen, 3) = 1 << 8;
	PA_BGXX(screen, 3) = 0;
	PA_BGXY(screen, 3) = 0;	

	PA_DrawBg[screen] =  (u16*)(0x06000000 + (0x200000 *  screen) + 128 * 256);
	DMA_Copy(Blank, (void*)PA_DrawBg[screen], 256*192, DMA_16NOW);
		
	//this is just used so we can write red color bits to one frame and green to the 
	//other
	
charsetstart[screen] = 2; // On se réserve la moitié de la mémoire...
charblocks[screen][16] = 1; // Block la mémoire

	_REG16(REG_BGSCREEN(screen)) &= ~7;
	_REG16(REG_BGSCREEN(screen)) |= (0x100 << (3)) | MODE_3_2D;
	_REG16(REG_BGCNT(screen, 3)) = bg_priority | BG_BMP16_256x256 | BG_BMP_BASE(2);

PA_SetDrawSize(screen, 1);
}



void PA_Draw16bitLine(u8 screen, u16 x1, u16 y1, u16 x2, u16 y2, u16 color){
  int i,dx,dy,sdx,sdy,dxabs,dyabs,x,y,px,py;

  dx=x2-x1;      /* the horizontal distance of the line */
  dy=y2-y1;      /* the vertical distance of the line */
  dxabs = dx;
  sdx = 1;
  if (dx < 0) {
	  dxabs = -dx;
	  sdx = -1;
  }
  dyabs = dy;
  sdy = 1;
  if (dy < 0) {
	  dyabs = -dy;
	  sdy = -1;
  }

  x=dyabs>>1;
  y=dxabs>>1;
  px=x1;
  py=y1;
  
PA_Put16bitPixel(screen, px, py, color);


  if (dxabs>=dyabs) {
    for(i=0;i<dxabs;i++)  {
      y+=dyabs;
      if (y>=dxabs)  {
        y-=dxabs;
        py+=sdy;
      }
      px+=sdx;
      PA_Put16bitPixel(screen, px, py, color);
    }
  }
  else {
    for(i=0;i<dyabs;i++) {
      x+=dxabs;
      if (x>=dyabs)  {
        x-=dyabs;
        px+=sdx;
      }
      py+=sdy;
      PA_Put16bitPixel(screen, px, py, color);
    }
  }
}

















void PA_8bitDraw(u8 screen, u8 color){
s16 i, j, low, high;
u16 x, y;

if (Stylus.Held){
	x = Stylus.X;
	y = Stylus.Y;
	i = 0;
	j = 0;
	
	low = (PA_drawsize[screen] >> 1) - PA_drawsize[screen];
	high = (PA_drawsize[screen] >> 1);

	// Si nouvelle pression, on fait juste un point. Sinon, on trace un trait entre les 2 points...
		if (Stylus.Newpress) {
			for (i = low; i < high; i++)
				for (j = low; j < high; j++)
					if ((x+i >= 0) && (y+j >= 0) && (x+i < 256) && (y+j < 192))
						PA_Put8bitPixel(screen, x+i, y+j, color);
		}
		else {
			PA_Draw8bitLineEx(screen, x, y, PA_oldx[screen], PA_oldy[screen], color, PA_drawsize[screen]);
		}
	
	PA_oldx[screen] = Stylus.X; PA_oldy[screen] = Stylus.Y;
}
}








void PA_16bitDraw(u8 screen, u16 color){
s16 i, j, low, high;
u16 x, y;

if (Stylus.Held){
	x = Stylus.X;
	y = Stylus.Y;
	i = 0;
	j = 0;
	
	low = (PA_drawsize[screen] >> 1) - PA_drawsize[screen];
	high = (PA_drawsize[screen] >> 1);

	// Si nouvelle pression, on fait juste un point. Sinon, on trace un trait entre les 2 points...
		if (Stylus.Newpress) {
			for (i = low; i < high; i++)
				for (j = low; j < high; j++)
					if ((x+i > 0) && (y+j > 0) && (x+i < 256) && (y+j < 192))
						PA_Put16bitPixel(screen, x+i, y+j, color);
		}
		else {
			PA_Draw16bitLineEx(screen, x, y, PA_oldx[screen], PA_oldy[screen], color, PA_drawsize[screen]);
		}
	
	PA_oldx[screen] = Stylus.X; PA_oldy[screen] = Stylus.Y;
}
}





void PA_Draw16bitLineEx(u8 screen, s16 basex, s16 basey, s16 endx, s16 endy, u16 color, s8 size){
s8 low = (size >> 1) - size;
s8 high = (size >> 1);
s16 i, j;
s16 x1, x2, y1, y2;

for (i = low; i < high; i++){
	for (j = low; j < high; j++){
		if ((basex+i >= 0) && (basey+j >= 0)&&(basex+i < 256) && (basey+j < 192)){
			PA_Put16bitPixel(screen, basex+i, basey+j, color);
		}
	}
}


for (i = low; i < high; i++){
	j = low;
	x1 = basex+i; x2 = endx+i; y1 = basey+j; y2 = endy+j;
	while(x1 < 0) x1++;	while(x1 > 255) x1--;
	while(x2 < 0) x2++;	while(x2 > 255) x2--;
	while(y1 < 0) y1++;	while(y1 > 191) y1--;
	while(y2 < 0) y2++;	while(y2 > 191) y2--;		
	PA_Draw16bitLine(screen, x1, y1, x2, y2, color);
		
	j = high-1;
	x1 = basex+i; x2 = endx+i; y1 = basey+j; y2 = endy+j;
	while(x1 < 0) x1++;	while(x1 > 255) x1--;
	while(x2 < 0) x2++;	while(x2 > 255) x2--;
	while(y1 < 0) y1++;	while(y1 > 191) y1--;
	while(y2 < 0) y2++;	while(y2 > 191) y2--;
	PA_Draw16bitLine(screen, x1, y1, x2, y2, color);		
}

for (j = low; j < high; j++){
	i = low;
	x1 = basex+i; x2 = endx+i; y1 = basey+j; y2 = endy+j;
	while(x1 < 0) x1++;	while(x1 > 255) x1--;
	while(x2 < 0) x2++;	while(x2 > 255) x2--;
	while(y1 < 0) y1++;	while(y1 > 191) y1--;
	while(y2 < 0) y2++;	while(y2 > 191) y2--;	
	PA_Draw16bitLine(screen, x1, y1, x2, y2, color);
	i = high-1;
	x1 = basex+i; x2 = endx+i; y1 = basey+j; y2 = endy+j;
	while(x1 < 0) x1++;	while(x1 > 255) x1--;
	while(x2 < 0) x2++;	while(x2 > 255) x2--;
	while(y1 < 0) y1++;	while(y1 > 191) y1--;
	while(y2 < 0) y2++;	while(y2 > 191) y2--;	
	PA_Draw16bitLine(screen, x1, y1, x2, y2, color);		
}

}




void PA_Draw8bitLineEx(u8 screen, s16 basex, s16 basey, s16 endx, s16 endy, u8 color, s8 size){
s8 low = (size >> 1) - size;
s8 high = (size >> 1);
s16 i, j;
s16 x1, x2, y1, y2;

for (i = low; i < high; i++){
	for (j = low; j < high; j++){
		if ((basex+i >= 0) && (basey+j >= 0)&&(basex+i < 256) && (basey+j < 192)){
			PA_Put8bitPixel(screen, basex+i, basey+j, color);
		}
	}
}


for (i = low; i < high; i++){
	j = low;
	x1 = basex+i; x2 = endx+i; y1 = basey+j; y2 = endy+j;
	while(x1 < 0) x1++;	while(x1 > 255) x1--;
	while(x2 < 0) x2++;	while(x2 > 255) x2--;
	while(y1 < 0) y1++;	while(y1 > 191) y1--;
	while(y2 < 0) y2++;	while(y2 > 191) y2--;		
	PA_Draw8bitLine(screen, x1, y1, x2, y2, color);
		
	j = high-1;
	x1 = basex+i; x2 = endx+i; y1 = basey+j; y2 = endy+j;
	while(x1 < 0) x1++;	while(x1 > 255) x1--;
	while(x2 < 0) x2++;	while(x2 > 255) x2--;
	while(y1 < 0) y1++;	while(y1 > 191) y1--;
	while(y2 < 0) y2++;	while(y2 > 191) y2--;
	PA_Draw8bitLine(screen, x1, y1, x2, y2, color);		
}

for (j = low; j < high; j++){
	i = low;
	x1 = basex+i; x2 = endx+i; y1 = basey+j; y2 = endy+j;
	while(x1 < 0) x1++;	while(x1 > 255) x1--;
	while(x2 < 0) x2++;	while(x2 > 255) x2--;
	while(y1 < 0) y1++;	while(y1 > 191) y1--;
	while(y2 < 0) y2++;	while(y2 > 191) y2--;	
	PA_Draw8bitLine(screen, x1, y1, x2, y2, color);
	i = high-1;
	x1 = basex+i; x2 = endx+i; y1 = basey+j; y2 = endy+j;
	while(x1 < 0) x1++;	while(x1 > 255) x1--;
	while(x2 < 0) x2++;	while(x2 > 255) x2--;
	while(y1 < 0) y1++;	while(y1 > 191) y1--;
	while(y2 < 0) y2++;	while(y2 > 191) y2--;	
	PA_Draw8bitLine(screen, x1, y1, x2, y2, color);		
}

}





void PA_Draw16bitRect(u8 screen, s16 basex, s16 basey, s16 endx, s16 endy, u16 color){
s16 i, j;

// On met dans le bon ordre...
if (endx < basex) {
	i = basex;	basex = endx;	endx = i;
}


if (basex < 0) basex = 0;
if (endx > 255) endx = 255;
s16 lx = endx - basex;

if (lx){ // Si y'a une surface à faire
	if (endy < basey) {
		i = basey;	basey = endy;	endy = i;
	}
	if (basey < 0) basey = 0;
	if (endy > 191) endy = 191;
	
	
	u16 *start = (u16*)PA_DrawBg[screen] + basex;
	tempvar = color;	
	for (j = basey; j < endy; j++){
		DMA_Force(tempvar, (void*)(start + (j << 8)), lx, DMA_16NOW);
	}
}
}




void PA_LoadFSImage(u8 screen, s16 FSImage){
	if (PA_CompareText(PA_FSFile[FSImage].Ext, "bmp")){
		PA_LoadBmp(screen, PA_PAFSFile(FSImage));
	}
	if (PA_CompareText(PA_FSFile[FSImage].Ext, "jpg")){ 
		PA_LoadJpeg(screen, PA_PAFSFile(FSImage));
	}	
	if (PA_CompareText(PA_FSFile[FSImage].Ext, "gif")){ 
		PA_LoadGif(screen, PA_PAFSFile(FSImage));
	}		
}








/*
void PA_DrawCircle(int xCenter, int yCenter, int radius, int color){
int x = 0;
int y = radius;
int p = 3 - 2 * radius;

   while (x <= y){
      PA_DrawPixel(xCenter + x, yCenter + y, color);
      PA_DrawPixel(xCenter - x, yCenter + y, color);
      PA_DrawPixel(xCenter + x, yCenter - y, color);
      PA_DrawPixel(xCenter - x, yCenter - y, color);
      PA_DrawPixel(xCenter + y, yCenter + x, color);
      PA_DrawPixel(xCenter - y, yCenter + x, color);
      PA_DrawPixel(xCenter + y, yCenter - x, color);
      PA_DrawPixel(xCenter - y, yCenter - x, color);
      if (p < 0) p += 4 * x++ + 6;
      else p += 4 * (x++ - y--) + 10;
   }
}
*/


#ifdef __cplusplus
}
#endif
