/*
SDLEngine
Matt Hoyle
*/
#pragma once
#include "thread.h"
#include "assert.h"
#include <SDL_thread.h>

namespace Kernel
{
	Thread::Thread()
		: m_handle(nullptr)
	{
	}

	Thread::~Thread()
	{
		WaitForFinish();
	}

	int32_t Thread::ThreadFn(void *ptr)
	{
		auto t = static_cast<Thread*>(ptr);
		SDE_ASSERT(t != nullptr);

		return t->m_function();
	}

	void Thread::Create(const char* name, TheadFunction fn)
	{
		SDE_ASSERT(m_handle == nullptr);
		m_function = fn;
		m_handle = SDL_CreateThread(ThreadFn, name, this);
		SDE_ASSERT(m_handle != nullptr);
	}

	int32_t Thread::WaitForFinish()
	{
		if (m_handle != nullptr)
		{
			int32_t result = 0;
			SDL_WaitThread(static_cast<SDL_Thread*>(m_handle), &result);
			m_handle = nullptr;
			return result;
		}
		else
		{
			return 0;
		}
	}
}
