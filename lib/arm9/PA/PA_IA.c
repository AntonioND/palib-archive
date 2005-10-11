#include <PA9.h>
u16 *maze[255];
u16 x, y;

u16 PA_Astar(u16 width, u16 height){
  int chemin[width][height];
  int bloc[width][height];
  int arriveex=0, arriveey=0, debutx=0, debuty=0;
  int i, j;
  for(j=0;j<height;j++)for(i=0;i<width;i++)bloc[i][j]=0;
  for(j=0;j<height;j++)for(i=0;i<width;i++){
  if(maze[i][j]==1){
  debutx = i;
  debuty = j;
  }
  if(maze[i][j]==2){
  maze[i][j]=0;
  arriveex = i;
  arriveey = j;
  }
  if(maze[i][j]==5){
  maze[i][j]=0;
  bloc[i][j]=1;
  }
  }
  while(1){
  if(maze[arriveex][arriveey]!=0)break;
  for(j=0;j<height;j++){
  for(i=0;i<width;i++){
  if(maze[i][j]>0){
  if(j<4)if(maze[i][j+1]==0&&bloc[i][j+1]==0)maze[i][j+1]=maze[i][j]+1;
  if(j>0)if(maze[i][j-1]==0&&bloc[i][j-1]==0)maze[i][j-1]=maze[i][j]+1;
  if(i<4)if(maze[i+1][j]==0&&bloc[i+1][j]==0)maze[i+1][j]=maze[i][j]+1;
  if(i>0)if(maze[i-1][j]==0&&bloc[i-1][j]==0)maze[i-1][j]=maze[i][j]+1;
  }
  }
  }
  }
  for(j=0;j<height;j++)for(i=0;i<width;i++)chemin[i][j]= 0;
  while(1){
  if(chemin[debutx][debuty]==1)break;
  for(i=0;i<height;i++){
  for(j=0;j<width;j++){
  chemin[arriveex][arriveey]=1;
  if(arriveey<4&&maze[arriveex][arriveey+1]<maze[arriveex][arriveey]&&bloc[arriveex][arriveey+1]==0)arriveey++;
  else if(arriveey>0&&maze[arriveex][arriveey-1]<maze[arriveex][arriveey]&&bloc[arriveex][arriveey-1]==0)arriveey--;
  else if(arriveex<4&&maze[arriveex+1][arriveey]<maze[arriveex][arriveey]&&bloc[arriveex+1][arriveey]==0)arriveex++;
  else if(arriveex>0&&maze[arriveex-1][arriveey]<maze[arriveex][arriveey]&&bloc[arriveex-1][arriveey]==0)arriveex--;
  }
  }
  }
  if(chemin[debutx-1][debuty]==1)return 1;
  else if(chemin[debutx+1][debuty]==1)return 2;
  else if(chemin[debutx][debuty-1]==1)return 3;
  else if(chemin[debutx][debuty+1]==1)return 4;
  return 0;
}

