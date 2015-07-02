/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "core/asset.h"
#include "core/asset_factory.h"

namespace Render
{
	class Material;

	class MaterialAssetFactory : public Core::AssetFactory
	{
	public:
		virtual ~MaterialAssetFactory() { };
		std::shared_ptr<Core::Asset> CreateAsset(std::string id) override;
	};

	class MaterialAsset : public Core::Asset
	{
	public:
		MaterialAsset(std::string id);
		~MaterialAsset();

		static const Core::Shortname c_assetType;

	private:
		bool Load(const rapidjson::Value& assetNode, const Core::AssetDatabase& db) override;
		std::string m_shaderProgramId;
		std::weak_ptr<Asset> m_shaderProgramAsset;
	};
}