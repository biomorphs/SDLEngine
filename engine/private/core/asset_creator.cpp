/*
SDLEngine
Matt Hoyle
*/
#include "asset_creator.h"
#include "asset_factory.h"

namespace Core
{
	AssetCreator::AssetCreator()
	{
	}

	AssetCreator::~AssetCreator()
	{
	}

	Asset* AssetCreator::CreateAsset(std::string id, Shortname assetType)
	{
		Asset* theAsset = nullptr;
		auto factoryIt = m_factories.find(assetType);
		if (factoryIt == m_factories.end())
		{
			return nullptr;
		}
		return factoryIt->second->CreateAsset(id);
	}
}