#include "PA9.h"


//////////////////////////////////////////////////////////////////////
// Buffer mode
//////////////////////////////////////////////////////////////////////

u16 PA_oldx[2], PA_oldy[2];
u8 PA_drawsize[2];
u16 *PA_DrawBg[2];

void PA_Init8bitBg(bool screen, u8 bg_priority){

PA_DeleteBg(screen, 3);

    //set the mode for 2 text layers and two extended background layers
	if (screen == 0) {
		//DISPLAY_CR = MODE_3_2D | DISPLAY_SPR_1D_LAYOUT | DISPLAY_SPR_ACTIVE | 2<<20; 
        BG3_XDX = 1 << 8;
        BG3_XDY = 0;
        BG3_YDX = 0;
        BG3_YDY = 1 << 8;
        BG3_CX = 0;
        BG3_CY = 0;	
	}
		
	else {
		//SUB_DISPLAY_CR = MODE_3_2D | DISPLAY_SPR_1D_LAYOUT | DISPLAY_SPR_ACTIVE | 2<<20;  
        SUB_BG3_XDX = 1 << 8;
        SUB_BG3_XDY = 0;
        SUB_BG3_YDX = 0;
        SUB_BG3_YDY = 1 << 8;
        SUB_BG3_CX = 0;
        SUB_BG3_CY = 0;	
	}

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



void PA_Draw8bitLine(bool screen, u16 x1, u16 y1, u16 x2, u16 y2, u8 color){
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



void PA_Init16bitBg(bool screen, u8 bg_priority){

PA_DeleteBg(screen, 3);

    //set the mode for 2 text layers and two extended background layers
	if (screen == 0) {
		//DISPLAY_CR = MODE_3_2D | DISPLAY_SPR_1D_LAYOUT | DISPLAY_SPR_ACTIVE | 2<<20; 
        BG3_XDX = 1 << 8;
        BG3_XDY = 0;
        BG3_YDX = 0;
        BG3_YDY = 1 << 8;
        BG3_CX = 0;
        BG3_CY = 0;	
	}
		
	else {
		//SUB_DISPLAY_CR = MODE_3_2D | DISPLAY_SPR_1D_LAYOUT | DISPLAY_SPR_ACTIVE | 2<<20;  
        SUB_BG3_XDX = 1 << 8;
        SUB_BG3_XDY = 0;
        SUB_BG3_YDX = 0;
        SUB_BG3_YDY = 1 << 8;
        SUB_BG3_CX = 0;
        SUB_BG3_CY = 0;	
	}

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


void PA_Draw16bitLine(bool screen, u16 x1, u16 y1, u16 x2, u16 y2, u16 color){
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








void PA_8bitDraw(bool screen, u16 color){
s8 i, j, low, high;
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
//		for (i = low; i < high; i++)
//			for (j = low; j < high; j++)
				if ((x+i < 256) && (y+j < 192)) PA_Put8bitPixel(screen, x+i, y+j, color);
	}
	else {
//		for (i = low; i < high; i++)
//			for (j = low; j < high; j++)
				if ((x+i < 256) && (y+j < 192) && (PA_oldx[screen]+i < 256) && (PA_oldy[screen]+j < 192)) PA_Draw8bitLine(screen, PA_oldx[screen]+i, PA_oldy[screen]+j, x+i, y+j, color);
	}

PA_oldx[screen] = Stylus.X; PA_oldy[screen] = Stylus.Y;
}
}


void PA_16bitDraw(bool screen, u16 color){
s8 i, j, low, high;
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
//		for (i = low; i < high; i++)
//			for (j = low; j < high; j++)
				if ((x+i < 256) && (y+j < 192)) PA_Put16bitPixel(screen, x+i, y+j, color);
	}
	else {
//		for (i = low; i < high; i++)
//			for (j = low; j < high; j++)
				if ((x+i < 256) && (y+j < 192) && (PA_oldx[screen]+i < 256) && (PA_oldy[screen]+j < 192)) PA_Draw16bitLine(screen, PA_oldx[screen]+i, PA_oldy[screen]+j, x+i, y+j, color);
	}

PA_oldx[screen] = Stylus.X; PA_oldy[screen] = Stylus.Y;
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



