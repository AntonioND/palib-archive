#ifndef _PA_IPC
#define _PA_IPC

#ifdef __cplusplus
extern "C" {
#endif


// Stylus and Pad definitions, will be used by the arm7 to auto-update positions...

typedef struct {
   u8 A, B, X, Y, L, R, Up, Down, Right, Left, Start, Select, Anykey;
} PA_Pad;

typedef struct {
   PA_Pad Held, Released, Newpress; // Pressed si on appuye, Held si on garde appuyé, et Released si on relache
} Pads;

typedef struct {
   u8 Held, Released, Newpress, Newpress0;
   s16 X, Y, altX, altY, Pressure, Vx, Vy, oldVx, oldVy, Downtime, Uptime, DblClick;
} PA_Stylus;



typedef struct {
	u8 Busy;
	u8 Volume;  // (1<<7) to change Volume, &127 for new Volume
	u8 Pan; // (1<<7) to change Pan, &127 to get value
} PA_IPCSound;

typedef struct{
	u8 *Data;
	s32 Length;
	u8 Volume; // Mic Volume
} PA_MicInfo;


typedef struct{
	PA_IPCSound Sound[17];
	PA_MicInfo Mic;
	PA_Stylus *Stylus;
	Pads *Pad;
} PA_IPCType;
//PA_IPCType PA_IPC;




#ifdef __cplusplus
}
#endif


#endif



