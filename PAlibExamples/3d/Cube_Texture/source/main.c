//copyright delfare <http://delfare.pizz.biz>
#include <PA9.h>
#include "PAlog.h"//cree a partir de palog.bin

int main(){
float rotatex = 0.0, rotatey = 0.0, rotatez = 0.0; 
PA_Init3D();
PA_LoadTexture(1,(u8*)PAlog,texBIN); 
while(1){ 
PA_Init3DDrawing(0.0,0.0,0.0,0.0,0.0,1.0);//initialisation de la perspective,... la camera est en position (0.0,0.0,2.0) et regarde vers (0.0,0.0,0.0) 
PA_Texture(1);
glPushMatrix();
rotatex += 2.0;
rotatey += 2.0;
rotatez += 2.0;
PA_3DBoxTexture(0.0,0.0,1.0,0.5,0.5,0.5,rotatex,rotatey,rotatez,255,0,0);
glPopMatrix(1);
glFlush();//afficher ce qu'on vient de dessiner 
}
} 
