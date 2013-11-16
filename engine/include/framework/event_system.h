/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "core/system.h"
#include <vector>

namespace Framework
{
	class IEventListener;

	// This class handles a list of systems that receive SDL events and 
	// passes events on each tick any registered listeners
	class EventSystem : public Core::ISystem
	{
	public:
		EventSystem();
		virtual ~EventSystem();

		void RegisterListener(IEventListener* listener);
		bool Tick();
		void Shutdown();

	private:
		typedef std::vector< IEventListener* > ListenerArray;
		ListenerArray m_listeners;
	};
}