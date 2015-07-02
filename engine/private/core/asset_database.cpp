/*
SDLEngine
Matt Hoyle
*/
#include "asset_database.h"
#include "kernel/assert.h"
#include "kernel/string_hashing.h"
#include "asset.h"

namespace Core
{
	AssetDatabase::AssetDatabase()
	{

	}

	AssetDatabase::~AssetDatabase()
	{

	}

	std::weak_ptr<Asset> AssetDatabase::AddAsset(std::shared_ptr<Asset> theAsset)
	{
		SDE_ASSERT(theAsset != nullptr);
		uint32_t assetIdHash = StringHashing::GetHash(theAsset->GetID().c_str());
		SDE_ASSERT(m_assets.find(assetIdHash) == m_assets.end());
		m_assets.insert({ assetIdHash, theAsset });
		return theAsset;
	}

	std::weak_ptr<Asset> AssetDatabase::GetAsset(std::string assetId) const
	{
		uint32_t assetIdHash = StringHashing::GetHash(assetId.c_str());
		auto theAsset = m_assets.find(assetIdHash);
		return theAsset->second;
	}

}