/*
SDLEngine
Matt Hoyle
*/
#include "timer.h"
#include <sdl_timer.h>

namespace Core
{
	Timer::Timer()
	{
		m_frequency = SDL_GetPerformanceFrequency();
	}

	Timer::~Timer()
	{
	}

	uint64_t Timer::GetTicks() const
	{
		return SDL_GetPerformanceCounter();
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