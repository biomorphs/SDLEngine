/*
SDLEngine
Matt Hoyle
*/
#include "job.h"
#include "kernel/assert.h"

namespace SDE
{
	Job::Job(JobSystem* parent, JobThreadFunction threadFn)
		: m_parent(parent)
		, m_threadFn(threadFn)
	{
		SDE_ASSERT(parent != nullptr);
	}

	Job::Job()
		: m_parent(nullptr)
	{

	}

	Job::~Job()
	{
		m_parent = nullptr;
	}

	void Job::Run()
	{
		m_threadFn();
	}
}