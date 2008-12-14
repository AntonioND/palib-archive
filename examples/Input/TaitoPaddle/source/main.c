#include <PA9.h>

#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"

int main(int argc, char ** argv)
{
  PA_Init();
  PA_InitVBL();

  PA_InitText(0, 0);
  PA_InitText(1, 0);

  PA_OutputSimpleText(1, 2, 1, "Taito Paddle demo for PAlib!");

// let's load some sprites to play with!
  u8 xPos = 127;
  PA_LoadSpritePal(0, 0, (void*)sprite0_Pal);
  PA_CreateSprite(0, 0, (void*)vaisseau_Sprite, OBJ_SIZE_32X32, 1, 0, xPos, 80);

  u16 angle = 0;
  PA_LoadSpritePal(1, 0, (void*)sprite0_Pal);
  PA_CreateSprite(1, 0, (void*)vaisseau_Sprite, OBJ_SIZE_32X32, 1, 0, 110, 80);
  PA_SetSpriteRotEnable(1, 0, 0); // because it's going to be rotated...

  PA_InitPaddle(); // call this first before trying to use the paddle

  while(1)
  {
    PA_OutputText(0, 1, 1, "Paddle : High Byte     : %d    ", Paddle.High); // the high byte of the whole 12-bit value (0-15)
    PA_OutputText(0, 1, 2, "Paddle : Low Byte      : %d    ", Paddle.Low); // the low byte of the whole 12-bit value (0-255)
    PA_OutputText(0, 1, 3, "Paddle : 12-Bit Value  : %d    ", Paddle.Count); // the calculated 12-bit value (0-4095)
    PA_OutputText(0, 1, 3, "Paddle : Velocity      : %d    ", Paddle.Speed); // the difference between successive values

    if(Paddle.Speed > 16) Paddle.Speed = 16; // you can limit values beyond a given threshhold for smoother movement...
    else if(Paddle.Speed < -16) Paddle.Speed = -16; // ...try experimenting with different threshhold numbers

    xPos += Paddle.Speed; // you can change a position based on the speed of paddle rotation...
    PA_SetSpriteX(0, 0, xPos);

    angle -= Paddle.Speed; // ...you can also change an angle
    angle &= 511; // limit the angle range to 0-511. (works only with 1, 3, 7, 15, 31, etc... (2^n  - 1))
    PA_SetRotsetNoZoom(1, 0, angle);

    PA_WaitForVBL(); // PA_WaitForVBL automatically updates the Paddle values
  }

  return 0;
}
