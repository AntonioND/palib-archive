
#ifdef __cplusplus
extern "C" {
#endif

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

u8 PA_MoveSpriteType = 0;

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
//u8 temp = (((~IPC->buttons) << 6) & (1<<12));
u8 temp = ((~IPC->buttons) >> 6) & 1;

	Stylus.Pressure = (((IPC->touchXpx * IPC->touchZ2) >> 6) / IPC->touchZ1) - (IPC->touchXpx >> 6);
	//if (Stylus.Pressure > 10) temp = 0; // limit to good pressures


	
	Stylus.Released = ((!temp) & Stylus.Held);
	Stylus.Newpress = temp & (!Stylus.Held);
	Stylus.Held = temp;
	
	
	Stylus.DblClick = Stylus.Newpress && (Stylus.Downtime+Stylus.Uptime < 45);
	Stylus.Downtime *= !Stylus.Newpress; // = 0 if newpress
	Stylus.Downtime += Stylus.Held;

	Stylus.Uptime *= !Stylus.Released; // = 0 when released
	Stylus.Uptime += !Stylus.Held;
	
	if (Stylus.Held) { // On en met à jour que si on touche l'écran, histoire de pas avoir un truc faussé
		Stylus.altX =  ((IPC->touchX - 0x0113) / 14);
		Stylus.altY =  ((IPC->touchY - 0x00E0) / 19);
	
		if(Stylus.Newpress){
			Stylus.X =  IPC->touchXpx;
			Stylus.Y =  IPC->touchYpx;
			Stylus.Vx = Stylus.oldVx = 0;
			Stylus.Vy = Stylus.oldVy = 0;
		}
		else if (PA_Distance (Stylus.oldVx, Stylus.oldVy, Stylus.Vx, Stylus.Vy) < 2500){ // Limit speed change
			Stylus.oldVx = Stylus.Vx;
			Stylus.oldVy = Stylus.Vy;	
			Stylus.Vx = IPC->touchXpx - Stylus.X;
			Stylus.Vy = IPC->touchYpx - Stylus.Y;					
			Stylus.X = IPC->touchXpx;
			Stylus.Y = IPC->touchYpx;
		}
		else {
			Stylus.Vx = Stylus.oldVx;
			Stylus.Vy = Stylus.oldVy;
		}
	}
		
	
}


u8 PA_MoveSpriteEx(u8 screen, u8 sprite, u8 lx, u8 ly) {
u8 truelx = (PA_GetSpriteLx(screen, sprite) >>1);
u8 truely = (PA_GetSpriteLy(screen, sprite) >>1);

s16 x = PA_GetSpriteX(screen, sprite) + truelx;
s16 y = PA_GetSpriteY(screen, sprite) + truely;
lx = lx >> 1;
ly = ly >> 1;

if (x >= 458) x -=511; // normalize the X coordinate...
if (y >= 220) y -=256; // normalize the X coordinate...


	if (Stylus.Released) {
		PA_MovedSprite.Moving = 0;
	}
	else{
		if (Stylus.Held & !PA_MovedSprite.Moving) { // Si nouvelle pression, on regarde si on touche ou pas le truc
			PA_MovedSprite.NextVx = 0;
			PA_MovedSprite.NextVx = 0;
			
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
			PA_MovedSprite.Vx = PA_MovedSprite.NextVx;
			PA_MovedSprite.Vy = PA_MovedSprite.NextVy;			
			PA_MovedSprite.NextVx = Stylus.X - PA_MovedSprite.X;
			PA_MovedSprite.NextVy = Stylus.Y - PA_MovedSprite.Y;

			PA_MovedSprite.X = Stylus.X;
			PA_MovedSprite.Y = Stylus.Y;
			PA_SetSpriteXY(screen, sprite, Stylus.X - truelx, Stylus.Y - truely);
			PA_MovedSprite.Time = 0; //Si on passe 2 vbl sans le bouger, on changera de cible
			return(1); // On a bougé...
		}
	}
	return(0);
}



u8 PA_MoveSpritePix(u8 sprite){

	if (Stylus.Released) {
		PA_MovedSprite.Moving = 0;
	}
	else{
		if (Stylus.Held & !PA_MovedSprite.Moving) { // Si nouvelle pression, on regarde si on touche ou pas le truc
			PA_MovedSprite.NextVx = 0;
			PA_MovedSprite.NextVx = 0;
			
			if ((PA_MoveSpriteType == 0) && PA_SpriteTouchedPix(sprite)){		//New sprite moving !
				PA_MovedSprite.Moving = 1;
				PA_MovedSprite.Sprite = sprite;
				PA_MovedSprite.X = PA_GetSpriteX(PA_Screen, sprite);
				PA_MovedSprite.Y = PA_GetSpriteY(PA_Screen, sprite);
				
			}
		}	
		else if ((!Stylus.Newpress) && PA_MovedSprite.Moving && (PA_MovedSprite.Sprite == sprite)) { // Si on peut le déplacer...
			PA_MovedSprite.Vx = PA_MovedSprite.NextVx;
			PA_MovedSprite.Vy = PA_MovedSprite.NextVy;			
			PA_MovedSprite.NextVx = Stylus.Vx;
			PA_MovedSprite.NextVy = Stylus.Vy;

			PA_MovedSprite.X += Stylus.Vx;
			PA_MovedSprite.Y += Stylus.Vy;
			PA_SetSpriteXY(PA_Screen, sprite, PA_MovedSprite.X, PA_MovedSprite.Y);
			PA_MovedSprite.Time = 0; //Si on passe 2 vbl sans le bouger, on changera de cible
			return(1); // On a bougé...
		}
	}
	return(0);
}










/*
u8 PA_MoveSpriteDistance(u8 sprite, u8 distance) {
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

#ifdef __cplusplus
}
#endif

