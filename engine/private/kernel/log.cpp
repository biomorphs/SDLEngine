/*
SDLEngine
	Matt Hoyle
*/

#include "log.h"
#include <stdio.h>
#include <stdarg.h>
#include <Windows.h>
#define WIN32_LEAN_AND_MEAN

namespace LogInternals
{
	// Log to the debug output window and the console
	void LogImpl(const char* txt, ...)
	{
		char logOutputBuffer[1024];
		va_list vl;
		va_start(vl, txt);
		vsprintf_s(logOutputBuffer, txt, vl);
		va_end(vl);
		strcat_s(logOutputBuffer, "\r\n");
		logOutputBuffer[1023] = { '\0' };
		OutputDebugString(logOutputBuffer);
		printf_s("%s", logOutputBuffer);
	}
}