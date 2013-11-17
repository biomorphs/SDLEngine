/*
SDLEngine
	Matt Hoyle
*/

#pragma once

namespace Core
{
	class ISystemRegistrar;
}

namespace Engine
{
	// This interface should be defined for each application / game / whatever
	class IAppSystemRegistrar
	{
	public:
		virtual void RegisterSystems(Core::ISystemRegistrar& registrar) = 0;
	};

	// This runs everything. Call it from main()!
	int Run(IAppSystemRegistrar& sysRegistrar, int argc, char* args[]);
}