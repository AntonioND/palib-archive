#ifndef _PA_IPC
#define _PA_IPC

#ifdef __cplusplus
extern "C" {
#endif



typedef struct {
	u16 MasterVol;
	u8 Busy;
	u8 Volume;  // New Volume
	u8 ChangeVolume; // 1 if change volume set...
} PA_IPCType;
//extern volatile PA_IPCType PA_IPC[16];




#ifdef __cplusplus
}
#endif


#endif



