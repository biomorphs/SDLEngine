/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "kernel/assert.h"
#include <SDL.h>
#include <glew.h>

// Renderer utils (global error handling, etc)

inline const char* TranslateGLError(uint32_t errorCode)
{
	switch (errorCode)
	{
	case GL_INVALID_VALUE:
		return "Invalid value";
	case GL_INVALID_ENUM:
		return "Invalid enum";
	case GL_INVALID_OPERATION:
		return "Invalid operation";
	case GL_OUT_OF_MEMORY:
		return "Out of memory";
	default:
		return "No errors";
	}
	return nullptr;
}

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

#define SDE_RENDER_PROCESS_GL_ERRORS(...)	\
	{\
		auto error = glGetError();	\
		while (error  != GL_NO_ERROR)	\
		{\
			SDE_RENDER_ASSERT(false, "A GL Error Occured: %s returned %d (%s)", ##__VA_ARGS__, error, TranslateGLError(error)); \
			error = glGetError();	\
		}\
	}

// Returns false if any errors occured
#define SDE_RENDER_PROCESS_GL_ERRORS_RET(...)	\
	{\
		bool shouldReturn = false;	\
		auto error = glGetError();	\
		while (error  != GL_NO_ERROR)	\
		{\
			SDE_RENDER_ASSERT(false, "A GL Error Occured: %s returned %d (%s)", ##__VA_ARGS__, error, TranslateGLError(error)); \
			error = glGetError();	\
			shouldReturn = true;	\
		}\
		if ( shouldReturn )	\
		{\
			return false;\
		}\
	}