/*
SDLEngine
	Matt Hoyle
*/

#include "kernel/assert.h"
#include "platform/platform.h"

// Application entry point
int main(int argc, char* args[])
{
	Platform::InitResult result = Platform::Initialise(argc, args);
	SDE_ASSERT(result == Platform::InitOK);
	if (result == Platform::InitFailed)
	{
		return 1;
	}

	Platform::ShutdownResult shutdownResult = Platform::Shutdown();
	SDE_ASSERT(shutdownResult == Platform::ShutdownOK);
	return shutdownResult;
}