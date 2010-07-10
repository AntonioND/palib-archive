#include <PA9.h>

infos PA_UserInfo;
RTC PA_RTC;

void PA_UpdateUserInfo(){
	int i;
	PA_UserInfo.Color = PersonalData->theme;
	PA_UserInfo.BdayMonth = PersonalData->birthMonth;
	PA_UserInfo.BdayDay = PersonalData->birthDay;
	PA_UserInfo.AlarmHour = PersonalData->alarmHour;
	PA_UserInfo.AlarmMinute = PersonalData->alarmMinute;
	PA_UserInfo.NameLength = PersonalData->nameLen;

	for (i = 0; i < PA_UserInfo.NameLength; i ++)
		PA_UserInfo.Name[i] = PersonalData->name[i];

	PA_UserInfo.Name[i] = 0; // To mark the end
	PA_UserInfo.MessageLength = PersonalData->messageLen;

	for (i = 0; i < PA_UserInfo.MessageLength; i++)
		PA_UserInfo.Message[i] = PersonalData->message[i];

	PA_UserInfo.Message[i] = 0; // To mark the end
	PA_UserInfo.Language = PersonalData->_user_data.language;
}

void PA_UpdateRTC(){
	time_t unixTime = time(NULL);
	struct tm* RTCV = gmtime((const time_t *) & unixTime);
	PA_RTC.Seconds = RTCV->tm_sec;
	PA_RTC.Minutes = RTCV->tm_min;
	PA_RTC.Hour = RTCV->tm_hour;
	PA_RTC.Day = RTCV->tm_mday;
	PA_RTC.Month = RTCV->tm_mon + 1;
	PA_RTC.Year = 1900 + RTCV->tm_year;
}
