/*
SDLEngine
Matt Hoyle
*/
#pragma once
#include "job.h"
#include "kernel/mutex.h"
#include <deque>

namespace SDE
{
	class JobQueue
	{
	public:
		JobQueue();
		~JobQueue();

		void PushJob(const Job& j);
		bool PopJob(Job &j);
		void RemoveAll();

	private:
		Kernel::Mutex m_lock;
		std::deque<Job> m_jobs;		// if allocations/frees become a problem, switch to pooled list
	};
}