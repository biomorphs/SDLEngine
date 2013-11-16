/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "SDL_events.h"

namespace Framework
{
	// This class represents an interface used to listen for SDL events
	class IEventListener
	{
	public:
		virtual void OnEventRecieved(const SDL_Event& theEvent) = 0;
	};
}