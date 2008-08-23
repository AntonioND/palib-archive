
#ifdef __cplusplus
extern "C" {
#endif

#include "PA9.h"
#include <arm9/PA_Keys.h>

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
				if (PA_MovedSprite.X >= 458) PA_MovedSprite.X -=511; // normalize the X coordinate...
				if (PA_MovedSprite.Y >= 220) PA_MovedSprite.Y -=256; // normalize the y coordinate...
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






#ifdef __cplusplus
}
#endif

