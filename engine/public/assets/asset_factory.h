/*
SDLEngine
Matt Hoyle
*/
#pragma once
#include <memory>

namespace Assets
{
	class Asset;
	class AssetFactory
	{
	public:
		AssetFactory() { }
		virtual ~AssetFactory() { }
		virtual Asset* CreateAsset(std::string id) = 0;
	};
}