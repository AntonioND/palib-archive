#include "PA9.h"



u16 RandomValue = 35329;



u32 PA_Rand(void) {

int i;


   RandomValue+=0x9248;

   for(i=0;i<3;i++)   {
       if(RandomValue&0x1) {
           RandomValue>>=1;
           RandomValue|=0x8000;
       }
       else  RandomValue>>=1;
   }

   return(RandomValue);
}




u16 PA_AdjustAngle(u16 angle, s16 anglerot, s32 startx, s32 starty, s32 targetx, s32 targety) {
u64 distances[3];

	startx = startx << 8; // Fixed point...
	starty = starty << 8; // Fixed point...
	targetx = targetx << 8; // Fixed point...
	targety = targety << 8; // Fixed point...

      u16 tempangle = (angle - anglerot) & 511;


      // Calcul des distances en fonction des angles
      distances[0] = PA_Distance(startx + PA_Cos(tempangle), starty - PA_Sin(tempangle), targetx, targety);
      tempangle += anglerot;
	  tempangle &= 511;
      distances[1] = PA_Distance(startx + PA_Cos(tempangle), starty - PA_Sin(tempangle), targetx, targety);
      tempangle += anglerot;
	  tempangle &= 511;
      distances[2] = PA_Distance(startx + PA_Cos(tempangle), starty - PA_Sin(tempangle), targetx, targety);
                                 
                                 

      // On regarde si l'angle est optimal. Si ce n'est pas le cas,
      // on fait tourner toujours dans le meme sens...
      if (distances[0] < distances[1])  angle -= anglerot;
      else if (distances[2] < distances[1])  angle += anglerot;
           
      return (angle&511);    
}

