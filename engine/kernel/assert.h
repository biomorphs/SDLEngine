/*
SDLEngine
	Matt Hoyle
*/
#pragma once

#include "log.h"

#define SDE_ASSERT(condition, ...)	if( !(condition) )	{ SDE_LOG("Assertion Failed!\r\n\t" #condition "\r\n\t" ##__VA_ARGS__); __debugbreak(); }