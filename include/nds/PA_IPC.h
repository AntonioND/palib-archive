#ifndef _PA_IPC
#define _PA_IPC

#ifdef __cplusplus
extern "C" {
#endif



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
} PA_IPCType;
//PA_IPCType PA_IPC;




#ifdef __cplusplus
}
#endif


#endif



