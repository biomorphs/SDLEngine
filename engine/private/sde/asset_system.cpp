/*
SDLEngine
Matt Hoyle
*/
#include "asset_system.h"
#include "assets/asset_serialiser.h"

namespace SDE
{
	AssetSystem::AssetSystem()
		: m_assetsRoot("assets")
	{
		m_pendingJobs.reserve(128);
		m_completedJobs.reserve(128);
		m_database = std::make_unique<Assets::AssetDatabase>();
	}

	AssetSystem::~AssetSystem()
	{
		SDE_ASSERT(m_pendingJobs.size() == 0);
		SDE_ASSERT(m_completedJobs.size() == 0);
	}

	std::shared_ptr<Assets::Asset> AssetSystem::GetAsset(const std::string& assetID)
	{
		return m_database->GetAsset(assetID);
	}

	void AssetSystem::LoadAsset(const char* assetName, std::function<void(const std::string&, bool)> onComplete)
	{
		LoadingJobDescriptor newJob;
		newJob.m_assetID = assetName;
		newJob.m_loadResult = false;
		newJob.m_then = onComplete;
		m_pendingJobs.push_back(newJob);
	}

	bool AssetSystem::Tick()
	{
		// fake job system
		for (auto &pending : m_pendingJobs)
		{
			Assets::AssetSerialiser serialiser(*m_database, m_creator);
			bool loadResult = serialiser.Load(m_assetsRoot.c_str(), pending.m_assetID.c_str());
			pending.m_loadResult = loadResult;
			m_completedJobs.push_back(pending);
		}
		m_pendingJobs.clear();

		// fake job completion
		for (auto& completed : m_completedJobs)
		{
			completed.m_then(completed.m_assetID, completed.m_loadResult);
		}
		m_completedJobs.clear();

		return true;
	}

	void AssetSystem::Shutdown()
	{
		// todo: wait on pending jobs, or cancel them?
		m_pendingJobs.clear();
		m_completedJobs.clear();

		// clear out the db, all assets should unload unless something leaks
		m_database = nullptr;
	}


}