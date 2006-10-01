#ifndef _PA_IPC
#define _PA_IPC

#ifdef __cplusplus
extern "C" {
#endif



typedef struct {
	u8 Busy;
	u8 Volume;  // New Volume
	u8 ChangeVolume; // 1 if change volume set... Also used for screenlight on DSL
} PA_IPCSound;

typedef struct{
	u8 *Data;
	s32 Length;
	u8 Volume; // Mic Volume
} PA_MicInfo;


typedef struct{
	PA_IPCSound Sound[17];
	PA_MicInfo Mic;
} PA_IPCType;
//PA_IPCType PA_IPC;




#ifdef __cplusplus
}
#endif


#endif



