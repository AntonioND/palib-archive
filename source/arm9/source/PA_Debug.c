#include "PA9.h"
#include "PA_Debug.h"
#include <stdarg.h>

//#define DISABLE_DEBUG

void _PA_iDeaS_OutputText(const char*);
void _PA_iDeaS_Breakpoint();

void PA_iDeaS_DebugOutput(const char* str){
#ifndef DISABLE_DEBUG
	if(PA_IsEmulator()) _PA_iDeaS_OutputText(str);
#endif
}

void PA_iDeaS_Breakpoint(){
#ifndef DISABLE_DEBUG
	if(PA_IsEmulator()) _PA_iDeaS_Breakpoint();
#endif
}

char _PA_DebugText[256];

void PA_iDeaS_DebugPrintf(const char* str, ...){
#ifndef DISABLE_DEBUG
	if(!PA_IsEmulator()) return;

	va_list args;

	va_start(args, 0);
	vsnprintf(_PA_DebugText, sizeof(_PA_DebugText), str, args);
	va_end(args);

	_PA_iDeaS_OutputText(_PA_DebugText);
#endif
}
