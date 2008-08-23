#include <PA9.h>

#define PA_DrawBigPixel(screen,x,y,zoom,color) PA_Draw16bitRect(screen, x*a-(a/2), y*a-(a/2),x*a+(a/2), y*a+(a/2), color)

int main(void){
	start:
	PA_Init();//init PAlib
	PA_InitVBL();//init the vbl
	int x = 0,y = 0,width=32,height=32,i,j, arriveex = 24, arriveey = 24, a=8;
	
	PA_InitText(1,2);//init the text in the up screen
	PA_SetBgPalCol(0, 0, PA_RGB(31, 31, 31)); // Set the bottom screen color to white
 	PA_Init16bitBg(0, 3);//init the 16bit mode in bottom screen
    PA_InitAstar(width, height);//init the Astar function
    for(j=0;j<width;j++)for(i=0;i<height;i++)maze[i][j]=0;//init lab
    PA_OutputText(1,1,1,"place the starting point and type start");
    while(Pad.Newpress.Start != 1){
    PA_Clear16bitBg(0);//clear the bottom screen
    x = Stylus.X/a;
    y = Stylus.Y/a;
    PA_DrawBigPixel(0, x, y,a, PA_RGB(31, 0, 0));
    PA_WaitForVBL();
    }
    PA_Clear16bitBg(0); //clear the bottom screen
    for(i=0;i<20;i++)PA_WaitForVBL();
    PA_OutputText(1,1,1,"place the point of end and type start        ");
    while(Pad.Newpress.Start != 1){
    PA_Clear16bitBg(0);//clear the bottom screen
    arriveex = Stylus.X/a;
    arriveey = Stylus.Y/a;
    PA_DrawBigPixel(0, arriveex, arriveey,a, PA_RGB(0, 31, 0));
    PA_WaitForVBL();
    }
    PA_Clear16bitBg(0);//clear the bottom screen
    PA_DrawBigPixel(0, x, y,a, PA_RGB(31, 0, 0));
    PA_DrawBigPixel(0, arriveex, arriveey,a, PA_RGB(0, 31, 0));
    for(i=0;i<20;i++)PA_WaitForVBL();
    for(i=0;i<192;i++){
    maze[0][i/a]=5;
    maze[248/a][i/a]=5;
    }
    for(i=0;i<256;i++){
    maze[i/a][0]=5;
    maze[i/a][192/a]=5;
    }
    PA_OutputText(1,1,1,"trace the labyrinth and type start           ");
    while(Pad.Newpress.Start != 1){
    maze[Stylus.X/a][Stylus.Y/a]=5;//5 = block
	PA_DrawBigPixel(0, Stylus.X/a, Stylus.Y/a,a, PA_RGB(0, 0, 31));
    PA_WaitForVBL();
    }
	maze[arriveex][arriveey]=2;//2 = end
	while(1){				
				
		maze[x][y]=1;//1 = begin
		for(j=0;j<height;j++)for(i=0;i<width;i++){
        if(maze[i][j]==1)PA_DrawBigPixel(0, i, j,a, PA_RGB(31, 0, 0));
        else if(maze[i][j]==2)PA_DrawBigPixel(0, i, j,a, PA_RGB(0, 31, 0));
        else if(maze[i][j]==5)PA_DrawBigPixel(0, i, j,a, PA_RGB(0, 0, 31));
		}
          switch(PA_Astar(width,height)){
			case 1: {
			maze[x][y]=0;
				x--;
				break;
			}
			case 2: {
			maze[x][y]=0;
				x++;
				break;
			}
			case 3:{
			maze[x][y]=0;
				y--;
				break;
			}
			case 4:{
			maze[x][y]=0;
				y++;
				break;
			case -1:
				PA_OutputSimpleText(1,5,10, "End of path or error :P");
				PA_OutputSimpleText(1,5,11, "        Press A        ");
				while(!Pad.Newpress.A) PA_WaitForVBL();
				goto start; //lol
				break;
			}
		}
		PA_WaitForVBL(); // wait half a second
		
		
	}
	return 0;
}

