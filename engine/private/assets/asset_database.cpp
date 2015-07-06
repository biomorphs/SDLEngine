/*
SDLEngine
Matt Hoyle
*/
#include "asset_database.h"
#include "kernel/assert.h"
#include "core/string_hashing.h"
#include "asset.h"

namespace Assets
{
	AssetDatabase::AssetDatabase()
	{

	}

	AssetDatabase::~AssetDatabase()
	{

	}

	void AssetDatabase::AddAsset(Asset* theAsset)
	{
		SDE_ASSERT(theAsset != nullptr);
		uint32_t assetIdHash = StringHashing::GetHash(theAsset->GetID().c_str());
		SDE_ASSERT(m_assets.find(assetIdHash) == m_assets.end());
		m_assets.insert({ assetIdHash, std::shared_ptr<Asset>(theAsset) });		// take ownership here
	}

	std::shared_ptr<Asset> AssetDatabase::GetAsset(std::string assetId) const
	{
		uint32_t assetIdHash = StringHashing::GetHash(assetId.c_str());
		auto theAsset = m_assets.find(assetIdHash);
		if (theAsset != m_assets.end())
		{
			return theAsset->second;
		}
		else
		{
			return nullptr;
		}
	}

}