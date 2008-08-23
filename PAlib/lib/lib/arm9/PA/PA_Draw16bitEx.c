#ifdef __cplusplus
extern "C" {
#endif


#include "PA9.h"



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







void PA_16bitDraw(u8 screen, u16 color){

// Si nouvelle pression, on fait juste un point. Sinon, on trace un trait entre les 2 points...
if (Stylus.Newpress) {
  s16 low = (PA_drawsize[screen] >> 1) - PA_drawsize[screen]+1;
  s16 high = (PA_drawsize[screen] >> 1)+1;
  s16 i =0, j = 0;

  for (i = low; i < high; i++)
	  for (j = low; j < high; j++)
	    if ((Stylus.X+i >= 0) && (Stylus.Y+j >= 0) && (Stylus.X+i < 256) && (Stylus.Y+j < 192))
		    PA_Put16bitPixel(screen, Stylus.X+i, Stylus.Y+j, color);
}
else if (Stylus.Held) {
  if(PA_olddowntime[screen] != (Stylus.Downtime-1)){PA_oldx[screen] = Stylus.X; PA_oldy[screen] = Stylus.Y;}

  PA_Draw16bitLineEx(screen, Stylus.X, Stylus.Y, PA_oldx[screen], PA_oldy[screen], color, PA_drawsize[screen]);
}

PA_oldx[screen] = Stylus.X; PA_oldy[screen] = Stylus.Y; PA_olddowntime[screen] = Stylus.Downtime;
}




void PA_Draw16bitLineEx(u8 screen, s16 basex, s16 basey, s16 endx, s16 endy, u16 color, s8 size){
s8 low = (size >> 1) - size +1;
s8 high = (size >> 1)+1;
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
	
	
	//u16 *start = (u16*)PA_DrawBg[screen] + basex;
	//tempvar = color;	
	for (j = basey; j < endy; j++){
		for(i = basex; i < endx; i++){
			PA_Put16bitPixel(screen, i, j, color);
		//DMA_Force(tempvar, (void*)(start + (j << 8)), lx, DMA_16NOW);
		}
	}
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
