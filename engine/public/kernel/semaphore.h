/*
SDLEngine
Matt Hoyle
*/
#pragma once
#include "base_types.h"

namespace Kernel
{
	class Semaphore
	{
	public:
		Semaphore(uint32_t initialValue);
		~Semaphore();

		void Post();
		void Wait();

	private:
		void* m_semaphore;
	};
}