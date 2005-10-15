/* Check out the user infos...
*/


#include <PA9.h>


// Main function
int main(void)

{
// PAlib init
PA_Init();
PA_InitVBL();

// Initialise the background text function
PA_InitText(1, 0);


// Main loop
while(1)
{
	// User name, and birthday (Day/Month)
	PA_OutputText(1, 2, 10, "User name : %s, %02d/%02d", PA_UserInfo.Name, PA_UserInfo.BdayDay, PA_UserInfo.BdayMonth);

	// Alarm Time
	PA_OutputText(1, 2, 12, "Alarm : %02d:%02d", PA_UserInfo.AlarmHour, PA_UserInfo.AlarmMinute);

	// DS Language... 0 si japonese, 1 is english, 2 is french...
	PA_OutputText(1, 2, 14, "Language : %d", PA_UserInfo.Language);
	
	// Special message
	PA_OutputText(1, 2, 16, "Message : %s", PA_UserInfo.Message);	


// In Pa_OutputText, %s is for strings, %d for numbers, and %02d means 'write the number, and if has less than
// 2 digits, complete with zeros... Example : 5 -> 05

	PA_WaitForVBL();

}
return 0;
}
