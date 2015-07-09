/*
SDLEngine
Matt Hoyle
*/
#pragma once
#include "job.h"
#include "kernel/mutex.h"
#include "core/list.h"
#include "core/object_pool.h"
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
		typedef Core::ListNode<Job> JobListNode;
		Core::ObjectPool<JobListNode> m_jobsPool;
		Core::List<JobListNode> m_jobs;
		static const uint32_t c_maxJobsQueued = 64 * 1024;
	};
}