#include "PA_Internal.h"

// Some pseudocode for this function
// micvol(x) uses static variable oldsmp = 0
// micvol(x) = (oldsmp + smpfix(x)) / 2, oldsmp = smpfix(x)
// smpfix(x) = abs(x) * 2
u8 PA_ReadMicVol(){
	static u8 oldsmp = 0;
	register s8 smp_s = micReadData8() ^ 0x80;
	register u8 smp = ((smp_s < 0) ? (-smp_s) : smp_s) << 1;
	return oldsmp = (oldsmp + smp) >> 1;
}

void PA_InputGetAndSend(){
	// The following is already managed by libnds:
	// Keys, Stylus, RTC, Powerdown, Screen lights, Led blink
	inputGetAndSend(); // a piece of cake isn't it?

	// Handle the remaining things (battery, temperature, microphone volume)
	PA_FifoMsg msg;
	msg.type = PA_MSG_INPUT;

	// Battery
	msg.InputMsg.battery = touchRead(TSC_MEASURE_BATTERY);

	// Temperature
	int t1, t2;
	msg.InputMsg.temperature = touchReadTemperature(&t1, &t2);
	msg.InputMsg.tdiode1 = t1;
	msg.InputMsg.tdiode2 = t2;
	
	// Microphone volume
	msg.InputMsg.micvol = PA_ReadMicVol();

	// Send the data
	PA_SendFifoMsg(msg);
}
