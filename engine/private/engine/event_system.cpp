/*
SDLEngine
Matt Hoyle
*/
#include "event_system.h"
#include "kernel/system_events.h"

namespace Engine
{
	EventSystem::EventSystem()
	{

	}

	EventSystem::~EventSystem()
	{

	}

	bool EventSystem::Tick()
	{
		Kernel::SystemEvents::Event theEvent = Kernel::SystemEvents::Event::None;
		bool quitRequested = false;
		while (Kernel::SystemEvents::PollEvent(theEvent))
		{
			switch (theEvent)
			{
			case Kernel::SystemEvents::Event::QuitRequest:
				quitRequested = true;
				break;
			}
		}
		return !quitRequested;
	}
}