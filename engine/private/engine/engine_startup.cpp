/*
SDLEngine
	Matt Hoyle
*/

#include "engine_startup.h"
#include "event_system.h"
#include "core/system_manager.h"
#include "kernel/platform.h"
#include "kernel/assert.h"

namespace Engine
{
	const char c_EventSystemName[] = "_Reserved_EventSystem";

	// Application entry point
	int Run(IAppSystemRegistrar& sysRegistrar, int argc, char* args[])
	{
		// Initialise platform stuff
		Kernel::Platform::InitResult result = Kernel::Platform::Initialise(argc, args);
		SDE_ASSERT(result == Kernel::Platform::InitResult::InitOK);
		if (result == Kernel::Platform::InitResult::InitFailed)
		{
			return 1;
		}

		// Create the system manager and register systems
		Core::SystemManager sysManager;

		// Always add an event system, since we always need it and the user never
		// iteracts with it
		sysManager.RegisterSystem(c_EventSystemName, new EventSystem());

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
		Kernel::Platform::ShutdownResult shutdownResult = Kernel::Platform::Shutdown();
		SDE_ASSERT(shutdownResult == Kernel::Platform::ShutdownResult::ShutdownOK);
		return shutdownResult == Kernel::Platform::ShutdownResult::ShutdownOK ? 0 : 1;
	}
}