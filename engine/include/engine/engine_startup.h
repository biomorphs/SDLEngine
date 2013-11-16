/*
SDLEngine
	Matt Hoyle
*/

#pragma once

namespace Core
{
	class SystemManager;
}

namespace Engine
{
	// This interface should be defined for each application / game / whatever
	class ISystemRegistrar
	{
	public:
		virtual void RegisterSystems(Core::SystemManager& systemManager) = 0;
	};

	// This runs everything. Call it from main()!
	int Run(ISystemRegistrar& sysRegistrar, int argc, char* args[]);
}