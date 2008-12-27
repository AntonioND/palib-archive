#ifdef __cplusplus
extern "C" {
#endif

#include "PA9.h"

#define UPDATEGHPAD(type, pad)\
	type.Green = (pad & BUTTON_GREEN) >> 6;\
	type.Red = (pad & BUTTON_RED) >> 5;\
	type.Yellow = (pad & BUTTON_YELLOW) >> 4;\
	type.Blue = (pad & BUTTON_BLUE) >> 3;\
	type.Anykey = (!(!((pad&120))));

#define UPDATEGHPADTIMES(button)\
	GHPad.DblClick.button = GHPad.Newpress.button && (GHPad.Downtime.button + GHPad.Uptime.button < 45);\
	GHPad.Downtime.button *= !GHPad.Newpress.button;\
	GHPad.Downtime.button += GHPad.Held.button;\
	GHPad.Uptime.button *= !GHPad.Released.button;\
	GHPad.Uptime.button += !GHPad.Held.button;


GH_Pad_Struct GHPad;
u16 GHCompletePad, GHExPad;
u8 gh_update_padtimes;

extern funcpointer GHPadVBL;

bool PA_DetectGHPad(void)
{
	WAIT_CR &= ~0x8080;
	if((GBA_BUS[0] == 0xF9FF) && (GBA_BUS[1] == 0xF9FF) && (GBA_BUS[2] == 0xF9FF) && (GBA_BUS[3] == 0xF9FF)) return 1;
	WAIT_CR &= ~0x8000;
	return 0;
}

bool PA_InitGHPad(void)
{
	GHPad.Uptime.Green  = 0; GHPad.Downtime.Green  = 0;
	GHPad.Uptime.Red    = 0; GHPad.Downtime.Red    = 0;
	GHPad.Uptime.Yellow = 0; GHPad.Downtime.Yellow = 0;
	GHPad.Uptime.Blue   = 0; GHPad.Downtime.Blue   = 0;
	GHPad.Uptime.Anykey = 0; GHPad.Downtime.Anykey = 0;

	if(PA_DetectGHPad())
	{
		GHPadVBL = PA_UpdateGHPad;
		return 1;
	}
	return 0;
}

void PA_DeInitGHPad(void)
{
	WAIT_CR &= ~0x8000;
	GHPadVBL = PA_Nothing;
}

void PA_UpdateGHPad(void)
{
	GHExPad = GHCompletePad;
	GHCompletePad = ~(GH_POLL );
	UPDATEGHPAD(GHPad.Released, (GHExPad & (~GHCompletePad)));
	UPDATEGHPAD(GHPad.Newpress, (GHCompletePad & (~GHExPad)));
	UPDATEGHPAD(GHPad.Held, GHCompletePad);

	if (gh_update_padtimes)
	{
		UPDATEGHPADTIMES(Green);
		UPDATEGHPADTIMES(Red);
		UPDATEGHPADTIMES(Yellow);
		UPDATEGHPADTIMES(Blue);
		UPDATEGHPADTIMES(Anykey);
	}
}



PaddleInfo Paddle;

extern funcpointer PaddleVBL;

bool PA_DetectPaddle(void)
{
	WAIT_CR &= ~0x8080;
	WAIT_CR |= 0x23;
	if((GBA_BUS[0] == 0xEFFF) && (GBA_BUS[1] == 0xEFFF) && (GBA_BUS[2] == 0xEFFF) && (GBA_BUS[3] == 0xEFFF)) return 1;
	WAIT_CR &= ~0x8000;
	return 0;
}

bool PA_InitPaddle(void)
{
	if(PA_DetectPaddle())
	{
		PaddleVBL = PA_UpdatePaddle;
		return 1;
	}
	return 0;
}

void PA_DeInitPaddle(void)
{
	WAIT_CR &= ~0x8000;
	PaddleVBL = PA_Nothing;
}

void PA_UpdatePaddle(void)
{
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
