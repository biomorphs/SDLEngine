/*
SDLEngine
Matt Hoyle
*/
#pragma once
#include "core/system.h"
#include "assets/asset_database.h"
#include "assets/asset_creator.h"
#include <functional>

namespace SDE
{
	class AssetSystem : public Core::ISystem
	{
	public:
		AssetSystem();
		virtual ~AssetSystem();

		std::shared_ptr<Assets::Asset> GetAsset(const std::string& assetID);
		void LoadAsset(const char* assetName, std::function<void(const std::string&, bool)> onComplete);
		Assets::AssetCreator& GetCreator() { return m_creator; }

		void Shutdown() override;
		bool Tick() override;

	private:
		struct LoadingJobDescriptor		// Async load data
		{
			std::string m_assetID;
			bool m_loadResult;
			std::function<void(const std::string&, bool)> m_then;	// called on MAIN THREAD once the job is complete
		};

		std::string m_assetsRoot;
		std::unique_ptr<Assets::AssetDatabase> m_database;
		Assets::AssetCreator m_creator;

		// Replace with job system!
		std::vector<LoadingJobDescriptor> m_pendingJobs;			
		std::vector<LoadingJobDescriptor> m_completedJobs;
	};
}