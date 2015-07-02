/*
SDLEngine
	Matt Hoyle
*/
#pragma once

#include "log.h"

#ifdef SDE_DEBUG

	#define SDE_ASSERT(condition, ...)	if( !(condition) )	{ SDE_LOGC(Engine,"Assertion Failed!\r\n\t" #condition "\r\n\t" ##__VA_ARGS__); __debugbreak(); }
	#define RAPIDJSON_ASSERT(x) SDE_ASSERT(x)

#else

	#define SDE_ASSERT(...)

#endif