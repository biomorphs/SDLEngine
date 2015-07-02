/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include <vector>
#include <memory>
#include "kernel/assert.h"
#include <rapidjson\document.h>

namespace Core
{
	class Asset;
	class AssetCreator;
	class AssetDatabase;

	class AssetSerialiser
	{
	public:
		AssetSerialiser(AssetDatabase& targetDb, AssetCreator& creator);
		~AssetSerialiser();

		bool Load(const char* assetsRoot, const char* assetName);

	private:
		bool ProcessDependencies(const char* assetsRoot, const rapidjson::Value& depsNode);
		bool LoadAsset(const char* assetsRoot, const rapidjson::Value& assetNode);
		std::string MakePath(const char* rootPath, const char* assetName) const;

		std::vector<std::string> m_loadedAssets;
		AssetCreator& m_assetCreator;
		AssetDatabase& m_db;
	};
}