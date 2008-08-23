
#ifdef __cplusplus
extern "C" {
#endif

#include <PA9.h>
u16 *maze[255];
//u16 mazex, mazey;

s8 PA_Astar(u16 width, u16 height){

u16 maze2[width][height];
u8 chemin[width][height];
u8 bloc[width][height];
u8 arriveex=0, arriveey=0, debutx=0, debuty=0;
u8 i, j;
u8 exit;
for(j=0;j<height;j++)for(i=0;i<width;i++)maze2[i][j]=maze[i][j];
for(j=0;j<height;j++)for(i=0;i<width;i++)bloc[i][j]=0;
for(j=0;j<height;j++)for(i=0;i<width;i++)
	{
	if(maze2[i][j]==1){  debutx = i;  debuty = j;  }
	if(maze2[i][j]==2){  maze2[i][j]=0;  arriveex = i;  arriveey = j;  }
	if(maze2[i][j]==5){  maze2[i][j]=0;  bloc[i][j]=1;  }
	}
while(1)
	{
	if(maze2[arriveex][arriveey]!=0)break;
	exit = 1;
	for(j=0;j<height;j++)	for(i=0;i<width;i++)
		{
		if(maze2[i][j]>0)
			{
			if(j<height-1 && maze2[i][j+1]==0 && bloc[i][j+1]==0) {maze2[i][j+1] = maze2[i][j]+1; exit = 0;}
			if(j>0 && maze2[i][j-1]==0 && bloc[i][j-1]==0) {maze2[i][j-1] = maze2[i][j]+1; exit = 0;}
			if(i<width-1 && maze2[i+1][j]==0 && bloc[i+1][j]==0) {maze2[i+1][j] = maze2[i][j]+1; exit = 0;}
			if(i>0 && maze2[i-1][j]==0 && bloc[i-1][j]==0) {maze2[i-1][j] = maze2[i][j]+1; exit = 0;}
			}
		}
	if(exit == 1) return -1;
	}
for(j=0;j<height;j++)for(i=0;i<width;i++)chemin[i][j]= 0;
while(1)
	{
	if(chemin[debutx][debuty]==1)break;
	chemin[arriveex][arriveey]=1;
	if(arriveey<height-1 && maze2[arriveex][arriveey+1] < maze2[arriveex][arriveey] && maze2[arriveex][arriveey+1] != 0 && bloc[arriveex][arriveey+1]==0) arriveey++;
	else if(arriveey>0 && maze2[arriveex][arriveey-1] < maze2[arriveex][arriveey] && maze2[arriveex][arriveey-1] != 0 && bloc[arriveex][arriveey-1]==0) arriveey--;
	else if(arriveex<width-1 && maze2[arriveex+1][arriveey] < maze2[arriveex][arriveey] && maze2[arriveex+1][arriveey] != 0 && bloc[arriveex+1][arriveey]==0) arriveex++;
	else if(arriveex>0 && maze2[arriveex-1][arriveey] < maze2[arriveex][arriveey] && maze2[arriveex-1][arriveey] != 0 && bloc[arriveex-1][arriveey]==0) arriveex--;
	}
if(chemin[debutx-1][debuty]==1)return 1;
else if(chemin[debutx+1][debuty]==1)return 2;
else if(chemin[debutx][debuty-1]==1)return 3;
else if(chemin[debutx][debuty+1]==1)return 4;
return 0;
}


#ifdef __cplusplus
}
#endif


