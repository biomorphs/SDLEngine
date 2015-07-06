/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "core/system.h"
#include <vector>

namespace Engine
{
	// This class handles polling system events and controls the lifetime of the engine
	// System-events(shutdown,etc) should not go outside this system
	class EventSystem : public Core::ISystem
	{
	public:
		EventSystem();
		virtual ~EventSystem();
		bool Tick();
	};
}