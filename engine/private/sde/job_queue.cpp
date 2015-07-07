/*
SDLEngine
Matt Hoyle
*/
#include "job_queue.h"

namespace SDE
{
	JobQueue::JobQueue()
	{

	}

	JobQueue::~JobQueue()
	{

	}

	void JobQueue::PushJob(const Job& j)
	{
		Kernel::ScopedMutex lock(m_lock);
		m_jobs.push_back(j);
	}

	bool JobQueue::PopJob(Job &j)
	{
		Kernel::ScopedMutex lock(m_lock);
		if (m_jobs.size() > 0)
		{
			j = m_jobs.front();
			m_jobs.erase(m_jobs.begin());
			return true;
		}
		return false;
	}

	void JobQueue::RemoveAll()
	{
		Kernel::ScopedMutex lock(m_lock);
		m_jobs.clear();
	}
}