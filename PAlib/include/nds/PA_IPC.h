#ifndef _PA_IPC
#define _PA_IPC

#ifdef __cplusplus
extern "C" {
#endif

// Bits...
#define PAIPC_PLAY 0
#define PAIPC_PAUSE 1
#define PAIPC_STOP 2


typedef struct {
	u8 Command; // Command bits : 0 play, 1 pause, 2 stop
	void *Data;
	u8 Busy;
	u8 Volume;  // (1<<7) to change Volume, &127 for new Volume
	u8 Pan; // (1<<7) to change Pan, &127 to get value
	s32 Rate;
	u32 Length;
	u8 Format;
	u8 Padding;
	u8 Repeat; // used to swtich between both systems for now
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



