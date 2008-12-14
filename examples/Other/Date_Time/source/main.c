/* Check out the date and the time
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

	// Day, Month, and Year
	PA_OutputText(1, 2, 10, "%02d/%02d/%02d", PA_RTC.Day, PA_RTC.Month, PA_RTC.Year);

	// And the time...
	PA_OutputText(1, 2, 12, "%02d:%02d  %02d seconds", PA_RTC.Hour, PA_RTC.Minutes, PA_RTC.Seconds);	

	PA_WaitForVBL();


}
return 0;
}
