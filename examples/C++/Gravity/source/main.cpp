//////////////////////////////////////////////////
// PAlib C++ project template                   //
// main.cpp                                     //
// Contains the entry point of the program.     //
//////////////////////////////////////////////////

#include <PA9.h>

#include "all_gfx.h"

#define JUMPFORCE (-4)
#define GRAVITY 0.25f

using PA::Sprite;
using PA::Fixed;

// Sprite with Y velocity
class VelSprite: public Sprite{
public:
	VelSprite(int a, int b) : Sprite(a, b){}
	Fixed vy;

	void vUpdate(){ pos.y += vy; }
	void custupdate(){ PA_SetSpriteXY(screen, sprid, pos.x - 15, pos.y - 31); }
};

VelSprite ball(1, 0);

int main(){
	PA_Init();

	PA_LoadSpritePal(1, 0, (void*) ball_Pal);
	ball.create((void*) ball_Sprite, OBJ_SIZE_32X32, 0);
	ball.move(128, 191);
	ball.vy = JUMPFORCE;

	for(;;){
		PA_WaitForVBlank();

		if(ball.pos.y < 192)
			// Constant floats are automatically converted
			// to fixed-point at compile-time 8)
			ball.vy += GRAVITY;

		ball.vUpdate();

		if(ball.pos.y >= 192)
			ball.vy = JUMPFORCE, ball.pos.y = 191;

		ball.custupdate();
	}
}
