/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "shortname.h"
#include <unordered_map>
#include <memory>

namespace Core
{
	class AssetFactory;
	class Asset;
	class AssetCreator
	{
	public:
		AssetCreator();
		~AssetCreator();
		
		template<class FactoryType>
		void RegisterFactory(Shortname typeName);

		std::shared_ptr<Asset> CreateAsset(std::string id, Shortname assetType);

	private:
		std::unordered_map< Shortname, std::unique_ptr<AssetFactory> > m_factories;
	};

	template<class FactoryType>
	void AssetCreator::RegisterFactory(Shortname typeName)
	{
		SDE_ASSERT(m_factories.find(typeName) == m_factories.end());
		m_factories[typeName] = std::make_unique<FactoryType>();
	}
}