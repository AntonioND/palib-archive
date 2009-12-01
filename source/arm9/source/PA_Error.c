#include <PA9.h>

void PA_Error(const char* text){
	PA_Init2D();
	PA_InitText(1, 0);
	PA_OutputSimpleText(1, 0, 0, text);
}

void _PA_Assert(const char* condition, const char* message, const char* file, int linen){
	PA_Init2D();
	PA_InitText(1, 0);
	PA_OutputSimpleText(1, 1, 1, "Assertion failed!");
	PA_OutputSimpleText(1, 1, 3, condition);
	PA_OutputSimpleText(1, 1, 5, message);
	PA_OutputText(1, 1, 7, "Line #%d in file:", linen);
	PA_OutputSimpleText(1, 1, 8, file);
	for(;;) swiWaitForVBlank();
}
