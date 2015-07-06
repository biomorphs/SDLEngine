/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "base_types.h"

namespace Kernel
{
	namespace Time
	{
		uint64_t HighPerformanceCounterTicks();
		uint64_t HighPerformanceCounterFrequency();
	}
}