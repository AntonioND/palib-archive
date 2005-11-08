#include <nds.h>
#include <math.h>
#include "PA9.h"
#include "palib.h"

int textureID;
float angle3dX = 0.5,angle3dY = 0.0, viseeX = 0.0, viseeY = 0.0, viseeZ = 1.0;
void PA_LoadSplash3D(){
float rotatex = 0.0, rotatey = 0.0, rotatez = 0.0; 
PA_Init3D();
PA_LoadTexture(1,(u8*)palib,texBIN); 
while(1){ 
PA_UpdatePad();
PA_Init3DDrawing(0.0,0.0,0.0,0.0,0.0,1.0);//initialisation de la perspective,... la camera est en position (0.0,0.0,2.0) et regarde vers (0.0,0.0,0.0) 
PA_Texture(1);
glPushMatrix();
rotatex += 2.0;
rotatey += 2.0;
rotatez += 2.0;
PA_3DBoxTexture(0.0,0.0,1.0,0.5,0.5,0.5,rotatex,rotatey,rotatez,255,0,0);
glPopMatrix(1);
glFlush();//afficher ce qu'on vient de dessiner 
if(Pad.Held.Start)break;
}
}
void PA_VueKeys(){
PA_UpdatePad();
		if(Pad.Held.Up) angle3dY += 0.01;
		if(Pad.Held.Down)angle3dY -= 0.01;
		if(Pad.Held.Left)angle3dX -= 0.01;
		if(Pad.Held.Right)angle3dX += 0.01;
		viseeX = sin(angle3dX);
		viseeZ = cos(angle3dX);
		viseeY = tan(angle3dY);
}
void PA_glulookat(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) 
{ 
   f32 eyex = floatof32(x1), eyey = floatof32(y1), eyez = floatof32(z1), lookAtx = floatof32(x2);
   f32 lookAty = floatof32(y2), lookAtz = floatof32(z2), upx = floatof32(x3), upy = floatof32(y3), upz = floatof32(z3);
   f32 side[3], forward[3], up[3]; 

   forward[0] = lookAtx - eyex; 
   forward[1] = lookAty - eyey; 
   forward[2] = lookAtz - eyez; 

   normalizef32(forward); 

   up[0] = upx; 
   up[1] = upy; 
   up[2] = upz; 

   crossf32(forward, up, side); 
   normalizef32(side); 
   crossf32(side, forward, up); 
    
   glMatrixMode(GL_MODELVIEW); 

   MATRIX_LOAD4x3 =  side[0]; 
   MATRIX_LOAD4x3 =  up[0]; 
   MATRIX_LOAD4x3 = -forward[0]; 

   MATRIX_LOAD4x3 =  side[1]; 
   MATRIX_LOAD4x3 =  up[1]; 
   MATRIX_LOAD4x3 = -forward[1]; 

   MATRIX_LOAD4x3 =  side[2]; 
   MATRIX_LOAD4x3 =  up[2]; 
   MATRIX_LOAD4x3 = -forward[2]; 

   MATRIX_LOAD4x3 = 0; 
   MATRIX_LOAD4x3 = 0; 
   MATRIX_LOAD4x3 = floatof32(-1.0); 

   glTranslate3f32(-eyex, -eyey, -eyez); 
}
void PA_Init3D(void){
powerON(POWER_ALL);
videoSetMode(MODE_0_3D);//le mode 3d
//PA_InitVBL();
glViewPort(0,0,255,191);
glClearColor(0,0,0);
glClearDepth(0x7FFF);
vramSetBankA(VRAM_A_TEXTURE);
}
void PA_Init3DDrawing(float x1, float y1, float z1, float x2, float y2, float z2){
glReset();
gluPerspective(35, 256.0 / 192.0, 0.1, 40);
		glLight(0, RGB15(31,31,31) , 0,				  floatov10(-1.0),		 0);
		glLight(1, RGB15(31,31,31) , 0,				  0,	floatov10(-1.0));
		glLight(2, RGB15(31,31,31) , 0,				  0,	floatov10(1.0));
PA_glulookat( x1, y1, z1,      //position de la camera
x2, y2, z2,     //regarder vers
0.0, 1.0, 0.0);//le haut
glMatrixMode(GL_TEXTURE);
glIdentity();
glMatrixMode(GL_MODELVIEW);
		glMaterialf(GL_AMBIENT, RGB15(16,16,16));
		glMaterialf(GL_DIFFUSE, RGB15(16,16,16));
		glMaterialf(GL_SPECULAR, BIT(15) | RGB15(8,8,8));
		glMaterialf(GL_EMISSION, RGB15(16,16,16));
		glMaterialShinyness();
		//not a real gl function and will likely change
		glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE  | POLY_FORMAT_LIGHT0| POLY_FORMAT_LIGHT1| POLY_FORMAT_LIGHT2);

}
void PA_3DBox(float x, float y, float z, float longueur, float largeur, float hauteur,float rotatex, float rotatey, float rotatez, uint8 red, uint8 green, uint8 blue){
glPushMatrix();
PA_Translate3D(x,y,z);
PA_Scale3D(longueur,largeur, hauteur);
PA_Rotate3D(rotatex, rotatey, rotatez);
glColor3b(red,green,blue);
glBegin(GL_QUAD);
//1
glVertex3v16(floatov16(1.0),floatov16(1.0),floatov16(1.0));
glVertex3v16(floatov16(-1.0),floatov16(1.0),floatov16(1.0));
glVertex3v16(floatov16(-1.0),floatov16(-1.0),floatov16(1.0));
glVertex3v16(floatov16(1.0),floatov16(-1.0),floatov16(1.0));
//2
glVertex3v16(floatov16(1.0),floatov16(1.0),floatov16(-1.0));
glVertex3v16(floatov16(-1.0),floatov16(1.0),floatov16(-1.0));
glVertex3v16(floatov16(-1.0),floatov16(-1.0),floatov16(-1.0));
glVertex3v16(floatov16(1.0),floatov16(-1.0),floatov16(-1.0));
//3
glVertex3v16(floatov16(-1.0),floatov16(1.0),floatov16(1.0));
glVertex3v16(floatov16(-1.0),floatov16(1.0),floatov16(-1.0));
glVertex3v16(floatov16(-1.0),floatov16(-1.0),floatov16(-1.0));
glVertex3v16(floatov16(-1.0),floatov16(-1.0),floatov16(1.0));
//4
glVertex3v16(floatov16(1.0),floatov16(1.0),floatov16(1.0));
glVertex3v16(floatov16(1.0),floatov16(1.0),floatov16(-1.0));
glVertex3v16(floatov16(1.0),floatov16(-1.0),floatov16(-1.0));
glVertex3v16(floatov16(1.0),floatov16(-1.0),floatov16(1.0));
//5
glVertex3v16(floatov16(1.0),floatov16(1.0),floatov16(1.0));
glVertex3v16(floatov16(1.0),floatov16(1.0),floatov16(-1.0));
glVertex3v16(floatov16(-1.0),floatov16(1.0),floatov16(-1.0));
glVertex3v16(floatov16(-1.0),floatov16(1.0),floatov16(1.0));
//6
glVertex3v16(floatov16(1.0),floatov16(-1.0),floatov16(1.0));
glVertex3v16(floatov16(1.0),floatov16(-1.0),floatov16(-1.0));
glVertex3v16(floatov16(-1.0),floatov16(-1.0),floatov16(-1.0));
glVertex3v16(floatov16(-1.0),floatov16(-1.0),floatov16(1.0));
glEnd();
glPopMatrix(1);
}
void PA_3DBoxTexture(float x, float y, float z, float longueur, float largeur, float hauteur,float rotatex, float rotatey, float rotatez, uint8 red, uint8 green, uint8 blue){
glPushMatrix();
glTranslate3f32(floatof32(x),floatof32(y),floatof32(z));
PA_Scale3D(longueur,largeur, hauteur);
PA_Rotate3D(rotatex, rotatey, rotatez);
glColor3b(red,green,blue);
glBegin(GL_QUAD);
//1
PA_TexCoord(128.0,128.0);
glVertex3v16(floatov16(1.0),floatov16(1.0),floatov16(1.0));
PA_TexCoord(128.0,0.0);
glVertex3v16(floatov16(-1.0),floatov16(1.0),floatov16(1.0));
PA_TexCoord(0.0,0.0);
glVertex3v16(floatov16(-1.0),floatov16(-1.0),floatov16(1.0));
PA_TexCoord(0.0,128.0);
glVertex3v16(floatov16(1.0),floatov16(-1.0),floatov16(1.0));
//2
PA_TexCoord(128.0,0.0);
glVertex3v16(floatov16(1.0),floatov16(1.0),floatov16(-1.0));
PA_TexCoord(128.0,128.0);
glVertex3v16(floatov16(-1.0),floatov16(1.0),floatov16(-1.0));
PA_TexCoord(0.0,128.0);
glVertex3v16(floatov16(-1.0),floatov16(-1.0),floatov16(-1.0));
PA_TexCoord(0.0,0.0);
glVertex3v16(floatov16(1.0),floatov16(-1.0),floatov16(-1.0));
//3
PA_TexCoord(128.0,128.0);
glVertex3v16(floatov16(-1.0),floatov16(1.0),floatov16(1.0));
PA_TexCoord(128.0,0.0);
glVertex3v16(floatov16(-1.0),floatov16(1.0),floatov16(-1.0));
PA_TexCoord(0.0,0.0);
glVertex3v16(floatov16(-1.0),floatov16(-1.0),floatov16(-1.0));
PA_TexCoord(0.0,128.0);
glVertex3v16(floatov16(-1.0),floatov16(-1.0),floatov16(1.0));
//4
PA_TexCoord(128.0,0.0);
glVertex3v16(floatov16(1.0),floatov16(1.0),floatov16(1.0));
PA_TexCoord(128.0,128.0);
glVertex3v16(floatov16(1.0),floatov16(1.0),floatov16(-1.0));
PA_TexCoord(0.0,128.0);
glVertex3v16(floatov16(1.0),floatov16(-1.0),floatov16(-1.0));
PA_TexCoord(0.0,0.0);
glVertex3v16(floatov16(1.0),floatov16(-1.0),floatov16(1.0));
//5
PA_TexCoord(0.0,128.0);
glVertex3v16(floatov16(1.0),floatov16(1.0),floatov16(1.0));
PA_TexCoord(128.0,128.0);
glVertex3v16(floatov16(1.0),floatov16(1.0),floatov16(-1.0));
PA_TexCoord(128.0,0.0);
glVertex3v16(floatov16(-1.0),floatov16(1.0),floatov16(-1.0));
PA_TexCoord(0.0,0.0);
glVertex3v16(floatov16(-1.0),floatov16(1.0),floatov16(1.0));
//6
PA_TexCoord(128.0,128.0);
glVertex3v16(floatov16(1.0),floatov16(-1.0),floatov16(1.0));
PA_TexCoord(0.0,128.0);
glVertex3v16(floatov16(1.0),floatov16(-1.0),floatov16(-1.0));
PA_TexCoord(0.0,0.0);
glVertex3v16(floatov16(-1.0),floatov16(-1.0),floatov16(-1.0));
PA_TexCoord(128.0,0.0);
glVertex3v16(floatov16(-1.0),floatov16(-1.0),floatov16(1.0));
glEnd();
glPopMatrix(1);
}
