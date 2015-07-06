/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "core/system_enumerator.h"
#include "core/system_registrar.h"
#include <vector>
#include <map>

namespace Core
{
	class ISystem;

	// This class handles ownership and updates of systems
	class SystemManager : public ISystemEnumerator, public ISystemRegistrar
	{
	public:
		SystemManager();
		~SystemManager();

		// ISystemEnumerator
		virtual ISystem* GetSystem(const char* systemName);

		// ISystemRegistrar
		void RegisterSystem(const char* systemName, ISystem* theSystem);

		bool Initialise();
		bool Tick();
		void Shutdown();

	private:
		typedef std::vector<ISystem*> SystemArray;
		typedef std::map<uint32_t, ISystem*> SystemMap;
		typedef std::pair<uint32_t, ISystem*> SystemPair;

		SystemArray m_systems;
		SystemMap m_systemMap;
	};
}