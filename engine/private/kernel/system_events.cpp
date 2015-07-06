/*
SDLEngine
Matt Hoyle
*/
#include "system_events.h"
#include <SDL_events.h>

namespace Kernel
{
	namespace SystemEvents
	{
		// Translate any sdl events to kernel system events
		inline Event TranslateEvent(SDL_Event& sdlEvent)
		{
			switch (sdlEvent.type)
			{
			case SDL_QUIT:
				return Event::QuitRequest;
			default:
				return Event::None;
			}			
		}

		bool PollEvent(Event& eventOut)
		{
			SDL_Event theEvent;
			while (SDL_PollEvent(&theEvent))
			{
				Event result = TranslateEvent(theEvent);
				if (result != Event::None)
				{
					eventOut = result;
					return true;
				}
			}
			return false;	// no more events to process
		}
	}
}