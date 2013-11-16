/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include <vector>
#include <map>
#include "kernel/string_hashing.h"
#include "system_enumerator.h"

namespace Core
{
	class ISystem;

	// This class handles ownership and updates of systems
	class SystemManager : public ISystemEnumerator
	{
	public:
		SystemManager();
		~SystemManager();

		template< class TheSystem >
		void AddSystem(const char* name);
		virtual ISystem* GetSystem(const char* systemName);

		bool Initialise();
		bool Tick();
		void Shutdown();

	private:
		void AddSystemInternal(const char* name, ISystem* theSystem);

		typedef std::vector<ISystem*> SystemArray;
		typedef std::map<StringHash, ISystem*> SystemMap;
		typedef std::pair<StringHash, ISystem*> SystemPair;

		SystemArray m_systems;
		SystemMap m_systemMap;
	};

	template< class TheSystem >
	void SystemManager::AddSystem(const char* name)
	{
		TheSystem* sys = new TheSystem();
		AddSystemInternal(name, sys);
	}
}