/*
SDLEngine
Matt Hoyle
*/
#include "semaphore.h"
#include "assert.h"
#include <SDL_mutex.h>

namespace Kernel
{
	Semaphore::Semaphore(uint32_t initialValue)
	{
		m_semaphore = SDL_CreateSemaphore(initialValue);
		SDE_ASSERT(m_semaphore);
	}

	Semaphore::~Semaphore()
	{
		SDL_DestroySemaphore(static_cast<SDL_semaphore*>(m_semaphore));
		m_semaphore = nullptr;
	}

	void Semaphore::Post()
	{
		int32_t result = SDL_SemPost(static_cast<SDL_semaphore*>(m_semaphore));
		SDE_ASSERT(result == 0);
	}

	void Semaphore::Wait()
	{
		int32_t result = SDL_SemWait(static_cast<SDL_semaphore*>(m_semaphore));
		SDE_ASSERT(result == 0);
	}
}