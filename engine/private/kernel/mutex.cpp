/*
SDLEngine
Matt Hoyle
*/
#include "mutex.h"
#include "assert.h"
#include <SDL_mutex.h>

namespace Kernel
{
	Mutex::Mutex()
	{
		m_mutex = SDL_CreateMutex();
		SDE_ASSERT(m_mutex);
	}

	Mutex::Mutex(Mutex&& other)
	{
		if (this != &other)
		{
			m_mutex = other.m_mutex;
			other.m_mutex = nullptr;
		}
	}

	Mutex::~Mutex()
	{
		if (m_mutex != nullptr)
		{
			SDL_DestroyMutex(static_cast<SDL_mutex*>(m_mutex));
		}
	}

	void Mutex::Lock()
	{
		int32_t result = SDL_LockMutex(static_cast<SDL_mutex*>(m_mutex));
		SDE_ASSERT(result == 0);
	}

	void Mutex::Unlock()
	{
		int32_t result = SDL_UnlockMutex(static_cast<SDL_mutex*>(m_mutex));
		SDE_ASSERT(result == 0);
	}

	ScopedMutex::ScopedMutex(Mutex& target)
		: m_mutex(target)
	{
		m_mutex.Lock();
	}

	ScopedMutex::~ScopedMutex()
	{
		m_mutex.Unlock();
	}
}