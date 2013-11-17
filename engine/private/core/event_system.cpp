/*
SDLEngine
Matt Hoyle
*/
#include "event_system.h"
#include "event_listener.h"
#include "kernel/assert.h"

namespace Core
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

	bool EventSystem::TranslateEvent(void* sdlEvent, EngineEvent& resultEvent)
	{
		const SDL_Event& theEvent = *static_cast<const SDL_Event*>(sdlEvent);
		switch (theEvent.type)
		{
		case SDL_QUIT:
			resultEvent.m_type = EngineEvent::QuitRequest;
			return true;
		}
		return false;
	}

	bool EventSystem::Tick()
	{
		SDL_Event theEvent;
		while (SDL_PollEvent(&theEvent))
		{
			EngineEvent translatedEvent;
			if (TranslateEvent(&theEvent, translatedEvent))
			{
				for (auto it = m_listeners.begin(); it != m_listeners.end(); ++it)
				{
					(*it)->OnEventRecieved(translatedEvent);
				}
			}
		}
		return true;
	}

	void EventSystem::Shutdown()
	{
		m_listeners.clear();
	}
}