#include "PA9.h"
#include <arm9/PA_Keys.h>


//////////////////////////////////////////////////////////////////////
// Système de touches
//////////////////////////////////////////////////////////////////////


Pads Pad;
PA_Pad* PadPointer;

PA_Stylus Stylus;

u16 CompletePad, ExPad, TempPad;


PA_movingsprite  PA_MovedSprite; // Pour les sprites que l'on bouge...

bool PA_MoveSpriteType = 0;

void PA_UpdatePad(void) {
   ExPad = CompletePad;
   CompletePad = ~(PA_BUTTONS + (((IPC->buttons)&3) << 10));
   UPDATEPAD(Pad.Held, CompletePad);
   UPDATEPAD(Pad.Released, (ExPad & (~CompletePad)));
   UPDATEPAD(Pad.Newpress, (CompletePad & (~ExPad)));
//PA_BackupPad();   
//PA_OutputText(1,10,0,"up : %d%d",Pad.Newpress.Up, Pad.Held.Up);
//PA_OutputText(1,10,1,"up : %d%d",BackupPad.Newpress.Up, BackupPad.Held.Up);


}

void PA_UpdateStylus(void) {
//bool temp = (((~IPC->buttons) << 6) & (1<<12));
bool temp = ((~IPC->buttons) >> 6) & 1;

	#ifdef EMUSTYLUS   // Emulation of the stylus...
		temp = (IPC->touchX && IPC->touchY);
	#endif

	Stylus.Newpress = temp & (!Stylus.Held);
	Stylus.Released = (!temp) & Stylus.Held;
	Stylus.Held = temp;

	if (Stylus.Held) { // On en met à jour que si on touche l'écran, histoire de pas avoir un truc faussé
		Stylus.altX =  ((IPC->touchX - 0x0113) / 14);
		Stylus.altY =  ((IPC->touchY - 0x00E0) / 19);
		Stylus.X =  IPC->touchXpx;
		Stylus.Y =  IPC->touchYpx;
		#ifdef EMUSTYLUS
			Stylus.X =  Stylus.altX+14;
			float tempx = Stylus.X * 1.03;
			Stylus.X = (s16)tempx;
			if (Stylus.X < 0) Stylus.X = 0;
			else if (Stylus.X > 255) Stylus.X = 255;
			
			Stylus.Y =  Stylus.altY-2;
			float tempy = Stylus.Y * 1.09;
			Stylus.Y = (s16)tempy;
			if (Stylus.Y < 0) Stylus.Y = 0;
			else if (Stylus.Y > 191) Stylus.Y = 191;
		#endif
		
		Stylus.Pressure = (((Stylus.X * IPC->touchZ2) >> 6) / IPC->touchZ1) - (Stylus.X >> 6);
	}
}


bool PA_MoveSpriteEx(bool screen, u8 sprite, u8 lx, u8 ly) {
u8 truelx = (PA_GetSpriteLx(screen, sprite) >>1);
u8 truely = (PA_GetSpriteLy(screen, sprite) >>1);

s16 x = PA_GetSpriteX(screen, sprite) + truelx;
s16 y = PA_GetSpriteY(screen, sprite) + truely;
lx = lx >> 1;
ly = ly >> 1;

if (x >= 458) x -=511; // normalize the X coordinate...
if (y >= 220) y -=256; // normalize the X coordinate...


	if (Stylus.Released) PA_MovedSprite.Moving = 0;

	if (Stylus.Held & !PA_MovedSprite.Moving) { // Si nouvelle pression, on regarde si on touche ou pas le truc
		if ((PA_MoveSpriteType == 0) && (Stylus.X > x - lx) && (Stylus.X < x + lx) && (Stylus.Y > y - ly) && (Stylus.Y < y + ly)){			  // Square collision	
			PA_MovedSprite.Moving = 1;
			PA_MovedSprite.Sprite = sprite;
		}
		else if (PA_MoveSpriteType && (PA_Distance(Stylus.X, Stylus.Y, x, y) < lx*ly)){	// Distance collision		
			PA_MovedSprite.Moving = 1;
			PA_MovedSprite.Sprite = sprite;
		}
	}	


	if (PA_MovedSprite.Moving && (PA_MovedSprite.Sprite == sprite)) { // Si on peut le déplacer...
		PA_MovedSprite.Vx = Stylus.X - PA_MovedSprite.X;
		PA_MovedSprite.Vy = Stylus.Y - PA_MovedSprite.Y;
		PA_MovedSprite.X = Stylus.X;
		PA_MovedSprite.Y = Stylus.Y;
		PA_SetSpriteXY(screen, sprite, Stylus.X - truelx, Stylus.Y - truely);
		PA_MovedSprite.Time = 0; //Si on passe 2 vbl sans le bouger, on changera de cible
		return(1); // On a bougé...
	}
	return(0);
}


/*
bool PA_MoveSpriteDistance(u8 sprite, u8 distance) {
u8 truelx = (PA_GetSpriteLx(0, sprite) >>1);
u8 truely = (PA_GetSpriteLy(0, sprite) >>1);
s16 x = PA_GetSpriteX(0, sprite) + truelx;
s16 y = PA_GetSpriteY(0, sprite) + truely;



	if (Stylus.Released) PA_MovedSprite.Moving = 0;

	if (Stylus.Held & !PA_MovedSprite.Moving) { // Si nouvelle pression, on regarde si on touche ou pas le truc
		if (PA_Distance(Stylus.X, Stylus.Y, x, y) < distance*distance){			
			PA_MovedSprite.Moving = 1;
			PA_MovedSprite.Sprite = sprite;
		}
	}
	


	if (PA_MovedSprite.Moving && (PA_MovedSprite.Sprite == sprite)) { // Si on peut le déplacer...
		PA_MovedSprite.Vx = Stylus.X - PA_MovedSprite.X;
		PA_MovedSprite.Vy = Stylus.Y - PA_MovedSprite.Y;
		PA_MovedSprite.X = Stylus.X;
		PA_MovedSprite.Y = Stylus.Y;
		PA_SetSpriteXY(0, sprite, Stylus.X - truelx, Stylus.Y - truely);
		PA_MovedSprite.Time = 0; //Si on passe 2 vbl sans le bouger, on changera de cible
		return(1); // On a bougé...
	}
	return(0);
}
*/


