/*
SDLEngine
Matt Hoyle
*/
#include "timer.h"
#include "kernel/time.h"

namespace Core
{
	Timer::Timer()
	{
		m_frequency = Kernel::Time::HighPerformanceCounterFrequency();
	}

	Timer::~Timer()
	{
	}

	uint64_t Timer::GetTicks() const
	{
		return Kernel::Time::HighPerformanceCounterTicks();
	}

	double Timer::GetSeconds() const
	{
		return (double)GetTicks() / (double)m_frequency;
	}

	ScopedTimer::ScopedTimer(double& result)
		: m_result(result)
	{
		m_startTicks = m_timer.GetTicks();
	}

	ScopedTimer::~ScopedTimer()
	{
		m_result = (double)(m_timer.GetTicks() - m_startTicks) / (double)m_timer.GetFrequency();
	}
}