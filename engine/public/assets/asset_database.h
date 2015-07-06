/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include <memory>
#include <unordered_map>

namespace Assets
{
	class Asset;
	class AssetDatabase
	{
	public:
		AssetDatabase();
		~AssetDatabase();

		void AddAsset(Asset* theAsset);		// database takes ownership
		std::shared_ptr<Asset> GetAsset(std::string assetId) const;

	private:
		std::unordered_map<uint32_t, std::shared_ptr<Asset>> m_assets;
	};
}