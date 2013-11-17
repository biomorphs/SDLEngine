/*
SDLEngine
	Matt Hoyle
*/

#include "engine_startup.h"
#include "platform.h"
#include "core/system_manager.h"
#include "kernel/assert.h"

namespace Engine
{
	// Application entry point
	int Run(IAppSystemRegistrar& sysRegistrar, int argc, char* args[])
	{
		// Initialise platform stuff
		Platform::InitResult result = Platform::Initialise(argc, args);
		SDE_ASSERT(result == Platform::InitOK);
		if (result == Platform::InitFailed)
		{
			return 1;
		}

		// Create the system manager and register systems
		Core::SystemManager sysManager;

		SDE_LOGC(Engine, "Registering systems...");
		sysRegistrar.RegisterSystems(sysManager);

		// Run the engine
		SDE_LOGC(Engine, "Initialising systems...");
		bool initResult = sysManager.Initialise();
		SDE_ASSERT(initResult);
		if (initResult == true)
		{
			SDE_LOGC(Engine, "Running engine main loop");
			while (sysManager.Tick())
			{
			}
		}

		SDE_LOGC(Engine, "Shutting down systems");
		sysManager.Shutdown();

		// Shutdown
		Platform::ShutdownResult shutdownResult = Platform::Shutdown();
		SDE_ASSERT(shutdownResult == Platform::ShutdownOK);
		return shutdownResult;
	}
}