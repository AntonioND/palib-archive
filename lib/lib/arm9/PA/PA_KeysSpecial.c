#ifdef __cplusplus
extern "C" {
#endif

#include "PA9.h"

GH_Pad GHPad;

u16 GHCompletePad, GHExPad, GHTempPad;

extern funcpointer GHPadVBL;

bool PA_DetectGHPad(void){
  WAIT_CR &= ~0x8080;
  if((GBA_BUS[0] == 0xF9FF) && (GBA_BUS[1] == 0xF9FF) && (GBA_BUS[2] == 0xF9FF) && (GBA_BUS[3] == 0xF9FF)) return 1;
  WAIT_CR &= ~0x8000;
  return 0;
}

bool PA_InitGHPad(void){
  if(PA_DetectGHPad()) {
      GHPadVBL = PA_UpdateGHPad;
      return 1;
    } 
  return 0;
}

void PA_DeInitGHPad(void){
  WAIT_CR &= ~0x8000;
  GHPadVBL = PA_Nothing;
}

void PA_UpdateGHPad(void) {
  GHExPad = GHCompletePad;
  GHCompletePad = ~(GH_POLL );
  UPDATEGHPAD(GHPad.Held, GHCompletePad);
  UPDATEGHPAD(GHPad.Released, (GHExPad & (~GHCompletePad)));
  UPDATEGHPAD(GHPad.Newpress, (GHCompletePad & (~GHExPad)));
}



PaddleInfo Paddle;

extern funcpointer PaddleVBL;

bool PA_DetectPaddle(void){
  WAIT_CR &= ~0x8080;
  WAIT_CR |= 0x23;
  if((GBA_BUS[0] == 0xEFFF) && (GBA_BUS[1] == 0xEFFF) && (GBA_BUS[2] == 0xEFFF) && (GBA_BUS[3] == 0xEFFF)) return 1;
  WAIT_CR &= ~0x8000;
  return 0;
}

bool PA_InitPaddle(void){
  if(PA_DetectPaddle())
  {
    PaddleVBL = PA_UpdatePaddle;
    return 1;
  }
  return 0;
}

void PA_DeInitPaddle(void){
  WAIT_CR &= ~0x8000;
  PaddleVBL = PA_Nothing;
}

void PA_UpdatePaddle(void) {
  Paddle.OldLow = Paddle.Low;
  Paddle.OldHigh = Paddle.High;
  Paddle.OldCount = Paddle.Count;

  Paddle.Low = PADDLE_LOW;
  Paddle.High = PADDLE_HIGH;
  Paddle.Count = (Paddle.High<<8)+Paddle.Low;

  Paddle.Speed = Paddle.Count - Paddle.OldCount;

  if(Paddle.High - Paddle.OldHigh == 15) Paddle.Speed -= 4096;
  else if(Paddle.High - Paddle.OldHigh == -15) Paddle.Speed += 4096;
}


#ifdef __cplusplus
}
#endif
