/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "kernel/base_types.h"

class StringHashing
{
public:
	// djb2 hash
	static inline uint32_t GetHash(const char* str)
	{
		uint32_t hash = 5381;

		int c=0;
		while( c = *str++ )
		{
			hash = ((hash << 5) + hash) + c;
		}

		return hash;
	}
};