

#ifdef __cplusplus
extern "C" {
#endif

#include <PA9.h>



infos PA_UserInfo;
RTC PA_RTC;  // Infos RTC...


void PA_UpdateUserInfo(void) {

u8 i;

 PA_UserInfo.Color = INFO_COLOR;
 PA_UserInfo.BdayMonth = INFO_BDAY_MONTH;
 PA_UserInfo.BdayDay = INFO_BDAY_DAY;
 PA_UserInfo.BdayMonth = INFO_BDAY_MONTH;
 PA_UserInfo.AlarmHour = INFO_ALARM_HOUR;
 PA_UserInfo.AlarmMinute = INFO_ALARM_MINUTE;

 PA_UserInfo.NameLength = INFO_NAME_LENGTH;
 for (i = 0; i < PA_UserInfo.NameLength; i++) PA_UserInfo.Name[i] = *(u8*)(0x027FFC86 + (i << 1));
PA_UserInfo.Name[i] = 0; // Pour marquer la fin...

 PA_UserInfo.MessageLength = INFO_MESSAGE_LENGTH;
 for (i = 0; i < PA_UserInfo.MessageLength; i++) PA_UserInfo.Message[i] = *(u8*)(0x027FFC9C + (i << 1));
PA_UserInfo.Message[i] = 0; // Pour marquer la fin...
PA_UserInfo.Language = (INFO_LANGUAGE)&7;
}





void PA_UpdateRTC(void) {

u8 i;
u8 *temp;
temp = (u8*)&PA_RTC;
	for (i = 0; i < 8; i++) temp[i] = IPC->time.curtime[i];

	if (PA_RTC.Hour > 12) PA_RTC.Hour -= 40;
 
	if (PA_RTC.OldSeconds != PA_RTC.Seconds){
		PA_RTC.FPS = PA_RTC.Frames;
		PA_RTC.Frames = 0;
		PA_RTC.OldSeconds = PA_RTC.Seconds;
	}

}



#ifdef __cplusplus
}
#endif
