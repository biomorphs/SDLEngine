/*
SDLEngine
	Matt Hoyle
*/

#include "kernel/assert.h"
#include "platform/platform.h"
#include "render/window.h"

// Application entry point
int main(int argc, char* args[])
{
	Platform::InitResult result = Platform::Initialise(argc, args);
	SDE_ASSERT(result == Platform::InitOK);
	if (result == Platform::InitFailed)
	{
		return 1;
	}

	{
		Render::Window appWindow(Render::Window::Properties("SDLEngine", 800, 600));
		appWindow.Show();
		appWindow.Hide();
	}

	Platform::ShutdownResult shutdownResult = Platform::Shutdown();
	SDE_ASSERT(shutdownResult == Platform::ShutdownOK);
	return shutdownResult;
}