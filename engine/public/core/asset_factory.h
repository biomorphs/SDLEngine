/*
SDLEngine
Matt Hoyle
*/
#pragma once
#include <memory>

namespace Core
{
	class Asset;
	class AssetFactory
	{
	public:
		AssetFactory() { }
		virtual ~AssetFactory() { }
		virtual std::shared_ptr<Asset> CreateAsset(std::string id) = 0;
	};
}