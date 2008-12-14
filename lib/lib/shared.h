// To avoid problems with using libnds's TransferRegion we will now use our own and append it after libnds's one

#ifndef _PA_CUSTOM_IPC
#define _PA_CUSTOM_IPC

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
typedef struct sTransferSound {
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
  TransferSoundData data[16];
  u8 count;
  u8 PADDING[3];
} TransferSound, * pTransferSound;

// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
typedef struct {
// ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~
	vint16 touchX,   touchY;	// TSC X, Y
	vint16 touchXpx, touchYpx;	// TSC X, Y pixel values
	vint16 touchZ1,  touchZ2;	// TSC x-panel measurements
	vuint16 tdiode1,  tdiode2; 	// TSC temperature diodes
	vuint32 temperature;		// TSC computed temperature

	uint16 buttons;
	uint16 battery;

	uint16 aux;

	union {
		vuint8 curtime[8];		// current time response from RTC

		struct {
			vu8 command;
			vu8 year;		//add 2000 to get 4 digit year
			vu8 month;		//1 to 12
			vu8 day;		//1 to (days in month)

			vu8 weekday;	// day of week
			vu8 hours;		//0 to 11 for AM, 52 to 63 for PM
			vu8 minutes;	//0 to 59
			vu8 seconds;	//0 to 59
		} rtc;
	} time;

	vuint32 mailData;
	vuint8 mailBusy;
	vuint32 mailSize;
	pTransferSound soundData;
} PA_TransferRegion;

extern inline PA_TransferRegion volatile * PA_getIPC() {
	return (PA_TransferRegion volatile *)(0x027FF000);
}

#undef IPC
#define IPC ((PA_TransferRegion volatile *)(0x027FF000))
#define getIPC PA_geIPC


#endif
