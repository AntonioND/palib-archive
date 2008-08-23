//copyright smealum http://jvteamds.free.fr
// Les Includes
#include <PA9.h>
#include "gfx/vaisseau.raw.c"
#include "gfx/master.pal.c"

//Fonction principale du code
int main(void){
	//Initialision de PAlib
	PA_Init();
	PA_InitText(0,0);
	PA_InitVBL();
	PA_LoadPal(PAL_SPRITE0, master_Palette);
	PA_CreateSprite(0, 0,(void*)vaisseau_Bitmap, OBJ_SIZE_32X32,1, 0, 0, 0);
	int x = 0,y = 0,a=1,i,j, arriveex = 4, arriveey = 4;
	
	PA_InitText(1,2);
	PA_InitText(0,2);
						
	while(1){
		if(Pad.Held.Up) a++;
		
		if(x == arriveex && y == arriveey){
			x=0;
			y=0;
		}
		for(j=0;j<5+a;j++)
			for(i=0;i<5+a;i++){
				PA_OutputText(1,i*2,j,"0");
			}
			
		PA_OutputText(1,x*2,y,"1");
		PA_OutputText(1,1*2,0,"5");
		PA_OutputText(1,1*2,1,"5");
		PA_OutputText(1,2*2,2,"5");
		PA_OutputText(1,2*2,3,"5");
		PA_OutputText(1,arriveex*2,arriveey,"2");
		
		PA_InitAstar(5+a, 5+a); // Initialise to the size of the mase you want...
		for(j=0;j<5+a;j++)
			for(i=0;i<5+a;i++)
				maze[i][j]=0;
				
		maze[x][y]=1;
		maze[arriveex][arriveey]=2;
		maze[1][0]=5;
		maze[1][1]=5;
		maze[2][2]=5;
		maze[2][3]=5;
		
		PA_OutputText(0,1,1,"        ");
		PA_OutputText(0,1,2,"%d",a);
		
		switch(PA_Astar(5+a,5+a)){
			case 1: {
				PA_OutputText(0,1,1,"Left   ");
				x--;
				break;
			}
			case 2: {
				PA_OutputText(0,1,1,"Right  ");
				x++;
				break;
			}
			case 3:{
				PA_OutputText(0,1,1,"Up     ");
				y--;
				break;
			}
			case 4:{
				PA_OutputText(0,1,1,"Down   ");
				y++;
				break;
			}
		}
		PA_SetSpriteXY(0,0,x*10,y*10);
		for(i=0;i<30; i++) PA_WaitForVBL(); // wait half a second
		
		
	}
	return 0;
}
