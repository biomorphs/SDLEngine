/*
SDLEngine
Matt Hoyle
*/
#include "job_queue.h"

namespace SDE
{
	JobQueue::JobQueue()
		: m_jobsPool(c_maxJobsQueued)
	{
	}

	JobQueue::~JobQueue()
	{
	}

	void JobQueue::PushJob(const Job& j)
	{
		Kernel::ScopedMutex lock(m_lock);
		auto jobNode = m_jobsPool.Allocate(j);	// Copy-construct job on allocate
		m_jobs.PushBack(jobNode);
	}

	bool JobQueue::PopJob(Job &j)
	{
		Kernel::ScopedMutex lock(m_lock);
		JobListNode* newJob = m_jobs.PopFront();
		if (newJob)
		{
			j = std::move(*newJob);
			m_jobsPool.Free(newJob);
			return true;
		}

		return false;
	}

	void JobQueue::RemoveAll()
	{
		Kernel::ScopedMutex lock(m_lock);

		JobListNode* j = nullptr;
		while ((j = m_jobs.PopFront()) != nullptr)
		{
			m_jobsPool.Free(j);
		}
	}
}