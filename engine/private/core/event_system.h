/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "core/system.h"
#include <vector>

namespace Core
{
	class IEventListener;
	class EngineEvent;

	// This class handles a list of systems that receive SDL events and 
	// passes engine events on each tick any registered listeners
	class EventSystem : public Core::ISystem
	{
	public:
		EventSystem();
		virtual ~EventSystem();

		void RegisterListener(IEventListener* listener);
		bool Tick();
		void Shutdown();

	private:
		void OnEventRecieved(const EngineEvent& theEvent) { }
		bool TranslateEvent(void* sdlEvent, EngineEvent& resultEvent);
		typedef std::vector< IEventListener* > ListenerArray;
		ListenerArray m_listeners;
	};
}