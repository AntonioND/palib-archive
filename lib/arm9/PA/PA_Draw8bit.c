#ifdef __cplusplus
extern "C" {
#endif


#include "PA9.h"




void PA_Draw8bitLineEx(u8 screen, s16 basex, s16 basey, s16 endx, s16 endy, u8 color, s8 size);



void PA_Default8bitInit(u8 screen, u8 bg_priority){
PA_DeleteBg(screen, 3);
PA_nBit[screen] = 0; // 8 bit

PA_BGXPA(screen, 3) = 1 << 8;
PA_BGXPB(screen, 3) = 0;
PA_BGXPC(screen, 3) = 0;
PA_BGXPD(screen, 3) = 1 << 8;
PA_BGXX(screen, 3) = 0;
PA_BGXY(screen, 3) = 0;	

	_REG16(REG_BGSCREEN(screen)) &= ~7;
	_REG16(REG_BGSCREEN(screen)) |= (0x100 << (3)) | MODE_3_2D;
}


void PA_Init8bitBg(u8 screen, u8 bg_priority){

PA_Default8bitInit(screen, bg_priority);


	PA_DrawBg[screen] =  (u16*)(0x06000000 + (0x200000 *  screen) + 320 * 256);
	DMA_Copy(Blank, (void*)PA_DrawBg[screen], 256*96, DMA_16NOW);
	
charsetstart[screen] = 5; // On se réserve la moitié de la mémoire...
charblocks[screen][40] = 1; // Block la mémoire

	_REG16(REG_BGCNT(screen, 3)) = bg_priority | BG_BMP8_256x256 | BG_BMP_BASE(5);
PA_SetDrawSize(screen, 1);
}



void PA_InitBig8bitBg(u8 screen, u8 bg_priority){

PA_Default8bitInit(screen, bg_priority); 

   PA_DrawBg[screen] =  (u16*)(0x06000000 + (0x200000 *  screen) + 256 * 256);
   DMA_Copy(Blank, (void*)PA_DrawBg[screen], 256*128, DMA_16NOW);
   
charsetstart[screen] = 4; // On se réserve la moitié de la mémoire...
charblocks[screen][32] = 1; // Block la mémoire

   _REG16(REG_BGCNT(screen, 3)) = bg_priority | BG_BMP8_256x256 | BG_BMP_BASE(4);
PA_SetDrawSize(screen, 1);
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



void PA_8bitDraw(u8 screen, u8 color){
s16 i, j, low, high;
u16 x, y;

if (Stylus.Held){
	x = Stylus.X;
	y = Stylus.Y;
	i = 0;
	j = 0;
	
	low = (PA_drawsize[screen] >> 1) - PA_drawsize[screen]+1;
	high = (PA_drawsize[screen] >> 1)+1;

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



void PA_Draw8bitLineEx(u8 screen, s16 basex, s16 basey, s16 endx, s16 endy, u8 color, s8 size){
s8 low = (size >> 1) - size+1;
s8 high = (size >> 1)+1;
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
