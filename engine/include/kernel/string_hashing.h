/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include <stdint.h>

typedef uint32_t StringHash;

class StringHashing
{
public:
	// djb2 hash
	static inline StringHash GetHash(const char* str)
	{
		StringHash hash = 5381;

		int c=0;
		while( c = *str++ )
		{
			hash = ((hash << 5) + hash) + c;
		}

		return hash;
	}
};