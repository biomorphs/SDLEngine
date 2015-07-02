/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include <memory>
#include <unordered_map>

namespace Core
{
	class Asset;
	class AssetDatabase
	{
	public:
		AssetDatabase();
		~AssetDatabase();

		std::weak_ptr<Asset> AddAsset(std::shared_ptr<Asset> theAsset);
		std::weak_ptr<Asset> GetAsset(std::string assetId) const;

	private:
		std::unordered_map<uint32_t, std::shared_ptr<Asset>> m_assets;
	};
}