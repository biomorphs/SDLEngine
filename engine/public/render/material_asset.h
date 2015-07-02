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
	class ShaderProgramAsset;

	class MaterialAssetFactory : public Core::AssetFactory
	{
	public:
		virtual ~MaterialAssetFactory() { };
		Core::Asset* CreateAsset(std::string id) override;
	};

	class MaterialAsset : public Core::Asset
	{
	public:
		MaterialAsset(std::string id);
		~MaterialAsset();

		static const Core::Shortname c_assetType;
		inline const Material* GetMaterial() const { return m_renderMaterial.get(); }

	private:
		bool Load(const rapidjson::Value& assetNode, const Core::AssetDatabase& db) override;
		std::string m_shaderProgramId;
		std::shared_ptr<Asset> m_shaderProgramAsset;
		std::unique_ptr<Material> m_renderMaterial;
	};
}