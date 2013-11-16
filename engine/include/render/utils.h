/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "kernel/assert.h"

// Renderer utils (global error handling, etc)

#define SDE_RENDER_ASSERT(condition, ...)				\
	if (!(condition))									\
	{													\
		const char* errorTxt = SDL_GetError();			\
		if (errorTxt != nullptr)						\
		{												\
			SDE_LOGC(Engine, "SDL Error: %s", errorTxt);\
		}												\
	}													\
	SDE_ASSERT(condition, ##__VA_ARGS__);