/*
SDLEngine
Matt Hoyle
*/
#include "event_system.h"
#include "event_listener.h"
#include "kernel/assert.h"

namespace Framework
{
	EventSystem::EventSystem()
	{

	}

	EventSystem::~EventSystem()
	{

	}

	void EventSystem::RegisterListener(IEventListener* listener)
	{
		SDE_ASSERT(listener);
		m_listeners.push_back(listener);
	}

	bool EventSystem::Tick()
	{
		SDL_Event theEvent;
		while (SDL_PollEvent(&theEvent))
		{
			for (auto it = m_listeners.begin(); it != m_listeners.end(); ++it)
			{
				(*it)->OnEventRecieved(theEvent);
			}
		}
		return true;
	}

	void EventSystem::Shutdown()
	{
		m_listeners.clear();
	}
}