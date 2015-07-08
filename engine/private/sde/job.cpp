/*
SDLEngine
Matt Hoyle
*/
#include "job.h"
#include "kernel/assert.h"

namespace SDE
{
	Job::Job(JobSystem* parent, JobThreadFunction threadFn, const char* dbgName)
		: m_parent(parent)
		, m_threadFn(threadFn)
		, m_dbgName(dbgName)
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