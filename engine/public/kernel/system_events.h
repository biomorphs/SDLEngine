/*
SDLEngine
Matt Hoyle
*/
#pragma once

namespace Kernel
{
	namespace SystemEvents
	{
		enum class Event
		{
			None,
			QuitRequest
		};

		bool PollEvent(Event& eventOut);
	}
}