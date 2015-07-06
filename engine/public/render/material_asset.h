/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "assets/asset.h"
#include "assets/asset_factory.h"

namespace Render
{
	class Material;
	class ShaderProgramAsset;

	class MaterialAssetFactory : public Assets::AssetFactory
	{
	public:
		virtual ~MaterialAssetFactory() { };
		Assets::Asset* CreateAsset(std::string id) override;
	};

	class MaterialAsset : public Assets::Asset
	{
	public:
		MaterialAsset(std::string id);
		~MaterialAsset();

		static const Core::Shortname c_assetType;
		inline const Material* GetMaterial() const { return m_renderMaterial.get(); }

	private:
		bool Load(const rapidjson::Value& assetNode, const Assets::AssetDatabase& db) override;
		std::string m_shaderProgramId;
		std::shared_ptr<Assets::Asset> m_shaderProgramAsset;
		std::unique_ptr<Material> m_renderMaterial;
	};
}