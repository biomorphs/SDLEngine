/*
SDLEngine
	Matt Hoyle
*/

#include "platform.h"
#include "kernel/log.h"
#include "kernel/assert.h"
#include "SDL.h"

Platform::InitResult Platform::Initialise(int argc, char* argv[])
{
	SDE_LOGC(Engine, "Initialising Platform");

	int sdlResult = SDL_Init(SDL_INIT_EVERYTHING);
	SDE_ASSERT(sdlResult == 0);

	if (sdlResult != 0)
	{
		SDE_LOGC(Engine, "Failed to initialise SDL:\r\n\t%s", SDL_GetError());
		return InitFailed;
	}

	return InitOK;
}

Platform::ShutdownResult Platform::Shutdown()
{
	SDE_LOGC(Engine, "Shutting down Platform");

	SDL_Quit();

	return ShutdownOK;
}