/*
SDLEngine
	Matt Hoyle
*/

#include "platform.h"

Platform::InitResult Platform::Initialise(int argc, char* argv[])
{
	return InitOK;
}

Platform::ShutdownResult Platform::Shutdown()
{
	return ShutdownOK;
}