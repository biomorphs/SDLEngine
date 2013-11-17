/*
SDLEngine
Matt Hoyle
*/
#include "system_manager.h"
#include "event_system.h"
#include "system.h"
#include "kernel/assert.h"
#include "kernel/log.h"

namespace Core
{
	const char c_EventSystemName[] = "_Reserved_EventSystem";

	SystemManager::SystemManager()
	{
		// The system manager always has an event system registered as the first thing
		// to run per-frame. This is so any events can be dispatched to all systems asap
		AddSystemInternal(c_EventSystemName, new EventSystem());
	}

	SystemManager::~SystemManager()
	{
	}

	void SystemManager::RegisterSystem(const char* systemName, ISystem* theSystem)
	{
		AddSystemInternal(systemName, theSystem);

		// All external systems receive engine events
		EventSystem* events = (EventSystem*)GetSystem(c_EventSystemName);
		events->RegisterListener(theSystem);
	}

	void SystemManager::AddSystemInternal(const char* name, ISystem* theSystem)
	{
		SDE_ASSERT(theSystem);
		StringHash nameHash = StringHashing::GetHash(name);
		SDE_ASSERT(m_systemMap.find(nameHash) == m_systemMap.end(), "A system already exists with this name");
		m_systems.push_back(theSystem);
		m_systemMap.insert(SystemPair(nameHash, theSystem));
	}

	ISystem* SystemManager::GetSystem(const char* systemName)
	{
		SystemMap::iterator it = m_systemMap.find(StringHashing::GetHash(systemName));
		if (it != m_systemMap.end())
		{
			return it->second;
		}

		return nullptr;
	}

	bool SystemManager::Initialise()
	{
		for (auto it = m_systems.begin(); it != m_systems.end(); ++it)
		{
			if (!(*it)->PreInit(*this))
			{
				return false;
			}
		}
		for (auto it = m_systems.begin(); it != m_systems.end(); ++it)
		{
			if (!(*it)->Initialise())
			{
				return false;
			}
		}
		for (auto it = m_systems.begin(); it != m_systems.end(); ++it)
		{
			if (!(*it)->PostInit())
			{
				return false;
			}
		}

		return true;
	}
	
	bool SystemManager::Tick()
	{
		for (auto it = m_systems.begin(); it != m_systems.end(); ++it)
		{
			if (!(*it)->Tick())
			{
				return false;
			}
		}
		return true;
	}
	
	void SystemManager::Shutdown()
	{
		for (auto it = m_systems.begin(); it != m_systems.end(); ++it)
		{
			(*it)->PreShutdown();
		}
		for (auto it = m_systems.begin(); it != m_systems.end(); ++it)
		{
			(*it)->Shutdown();
		}
		for (auto it = m_systems.begin(); it != m_systems.end(); ++it)
		{
			(*it)->PostShutdown();
		}
		for (auto it = m_systems.begin(); it != m_systems.end(); ++it)
		{
			delete (*it);
		}
		m_systems.clear();
		m_systemMap.clear();
	}
}