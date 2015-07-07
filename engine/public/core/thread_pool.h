/*
SDLEngine
Matt Hoyle
*/

#pragma once

#include "kernel/atomics.h"
#include <vector>
#include <memory>
#include <functional>

namespace Kernel
{
	class Thread;
}

namespace Core
{
	// A pool of threads, all of which call the same thread function repeatedly
	// until Stop() is called, or the ThreadPool is destroyed
	class ThreadPool
	{
	public:
		ThreadPool();
		~ThreadPool();

		typedef std::function<void()> ThreadPoolFn;

		void Start(const char* poolName, uint32_t threadCount, ThreadPoolFn threadfn);
		void Stop();

	private:
		class PooledThread;
		std::vector<std::unique_ptr<PooledThread>> m_threads;
		Kernel::AtomicInt32 m_stopRequested;
		ThreadPoolFn m_fn;
	};
}