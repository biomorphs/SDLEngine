/*
SDLEngine
Matt Hoyle
*/
#pragma once

namespace Kernel
{
	class Mutex
	{
	public:
		Mutex();
		Mutex(const Mutex& other) = delete;
		~Mutex();

		void Lock();
		void Unlock();

	private:
		void* m_mutex;
	};

	class ScopedMutex
	{
	public:
		ScopedMutex(Mutex& target);
		~ScopedMutex();
	private:
		Mutex& m_mutex;
	};
}