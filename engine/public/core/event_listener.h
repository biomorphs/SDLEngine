/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "SDL_events.h"

namespace Core
{
	// This class represents an engine-wide event. passed to all systems
	class EngineEvent
	{
	public:
		enum Type
		{
			None,
			QuitRequest
		};

		Type m_type;
	};

	// This class represents an interface used to listen for engine events
	class IEventListener
	{
	public:
		virtual void OnEventRecieved(const EngineEvent& theEvent) = 0;
	};

	// This class represents the interface used to register even listeners
	class IEvenListenerRegistrar
	{
	public:
		virtual void RegisterEventListener(IEventListener& listener) = 0;
	};
}