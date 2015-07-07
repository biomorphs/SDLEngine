/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include <functional>

namespace SDE
{
	class JobSystem;

	class Job
	{
	public:
		typedef std::function<void()> JobThreadFunction;	// Code to be ran on the job thread

		Job();
		Job(JobSystem* parent, JobThreadFunction threadFn);
		~Job();
		void Run();

	private:
		JobThreadFunction m_threadFn;
		JobSystem* m_parent;
	};
}