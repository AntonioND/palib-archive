
#ifdef __cplusplus
extern "C" {
#endif

#include "PA9.h"
#include <arm9/PA_Keys.h>


//////////////////////////////////////////////////////////////////////
// Système de touches
//////////////////////////////////////////////////////////////////////



extern u16 CompletePad, ExPad, TempPad;
#define UPDATEPAD(type, pad)  type.A = pad & BUTTON_A;\
   type.B = (pad & BUTTON_B) >> 1;\
   type.Select = (pad & BUTTON_SELECT) >> 2;\
   type.Start = (pad & BUTTON_START) >> 3;\
   type.Right = (pad & BUTTON_RIGHT) >> 4;\
   type.Left = (pad & BUTTON_LEFT) >> 5;\
   type.Up = (pad & BUTTON_UP) >> 6;\
   type.Down = (pad & BUTTON_DOWN) >> 7;\
   type.R = (pad & BUTTON_R) >> 8;\
   type.L = (pad & BUTTON_L) >> 9;\
   type.X = (pad & BUTTON_X) >> 10;\
   type.Y = (pad & BUTTON_Y) >> 11;\
   type.Anykey = (!(!((pad&4095))));
  /*
#define COPYPAD(new, old)  new.A = old.A;\
   new.B = old.B;\
   new.Select = old.Select;\
   new.Start = old.Start;\
   new.Right = old.Right;\
   new.Left = old.Left;\
   new.Up = old.Up;\
   new.Down = old.Down;\
   new.R = old.R;\
   new.L =  old.L;\
   new.X = old.X;\
   new.Y = old.Y;*/
   
   #define PA_BUTTONS (*(volatile u16*)0x04000130)

#ifndef REG_KEYCNT
	#define REG_KEYCNT (*(volatile u16*)0x04000132)
#endif

#define BUTTON_A 1
#define BUTTON_B 2
#define BUTTON_SELECT 4
#define BUTTON_START 8
#define BUTTON_RIGHT 16
#define BUTTON_LEFT 32
#define BUTTON_UP 64
#define BUTTON_DOWN 128
#define BUTTON_R 256
#define BUTTON_L 512
#define BUTTON_X 1024
#define BUTTON_Y 2048

u16 CompletePad, ExPad, TempPad;


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
			Stylus.Vx = Stylus.oldVx = 0;
			Stylus.Vy = Stylus.oldVy = 0;
		}
		else{// if (PA_Distance (Stylus.oldVx, Stylus.oldVy, Stylus.Vx, Stylus.Vy) < 2500){ // Limit speed change
			Stylus.oldVx = Stylus.Vx;
			Stylus.oldVy = Stylus.Vy;	
			Stylus.Vx = IPC->touchXpx - Stylus.X;
			Stylus.Vy = IPC->touchYpx - Stylus.Y;					
//			Stylus.X = IPC->touchXpx;
//			Stylus.Y = IPC->touchYpx;
		}
		/*else {
			Stylus.Vx = Stylus.oldVx;
			Stylus.Vy = Stylus.oldVy;
		}*/
		
		while (IPC->mailBusy);		
		Stylus.X =  IPC->touchXpx;
		Stylus.Y =  IPC->touchYpx;			
		
	}
		
	
}







#ifdef __cplusplus
}
#endif

