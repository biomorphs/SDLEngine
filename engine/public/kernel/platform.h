/*
SDLEngine
	Matt Hoyle
*/
#pragma once

namespace Kernel
{
	namespace Platform
	{
		enum class InitResult
		{
			InitOK,
			InitFailed,
		};

		enum class ShutdownResult
		{
			ShutdownOK
		};

		InitResult Initialise(int argc, char* argv[]);
		ShutdownResult Shutdown();
	}
}
