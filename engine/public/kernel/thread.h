/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "base_types.h"
#include <string>
#include <functional>

namespace Kernel
{
	class Thread
	{
	public:
		Thread();
		~Thread();

		typedef std::function<int32_t()> TheadFunction;

		void Create(const char* name, TheadFunction fn);	// Start the thread instantly
		int32_t WaitForFinish();							// Called in dtor, but can be used manually

	private:
		static int32_t ThreadFn(void *ptr);
		TheadFunction m_function;
		void* m_handle;
	};
}
