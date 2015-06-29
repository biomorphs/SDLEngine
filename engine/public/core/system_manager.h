/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "core/system_enumerator.h"
#include "core/system_registrar.h"
#include "kernel/string_hashing.h"
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
		virtual ISystem* GetSystem(const std::string& systemName);

		// ISystemRegistrar
		void RegisterSystem(const std::string& systemName, ISystem* theSystem);

		bool Initialise();
		bool Tick();
		void Shutdown();

	private:
		void AddSystemInternal(const std::string& name, ISystem* theSystem);

		typedef std::vector<ISystem*> SystemArray;
		typedef std::map<StringHash, ISystem*> SystemMap;
		typedef std::pair<StringHash, ISystem*> SystemPair;

		SystemArray m_systems;
		SystemMap m_systemMap;
	};
}