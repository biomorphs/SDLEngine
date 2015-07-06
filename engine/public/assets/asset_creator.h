/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "core\shortname.h"
#include <unordered_map>
#include <memory>

namespace Assets
{
	class AssetFactory;
	class Asset;
	class AssetCreator
	{
	public:
		AssetCreator();
		~AssetCreator();
		
		template<class FactoryType>
		void RegisterFactory(Core::Shortname typeName);

		Asset* CreateAsset(const std::string& id, Core::Shortname assetType);

	private:
		std::unordered_map< Core::Shortname, std::unique_ptr<AssetFactory> > m_factories;
	};

	template<class FactoryType>
	void AssetCreator::RegisterFactory(Core::Shortname typeName)
	{
		SDE_ASSERT(m_factories.find(typeName) == m_factories.end());
		m_factories[typeName] = std::make_unique<FactoryType>();
	}
}