/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "assets/asset.h"
#include "assets/asset_factory.h"

namespace Render
{
	class Texture;

	class TextureAssetFactory : public Assets::AssetFactory
	{
	public:
		virtual ~TextureAssetFactory() { };
		Assets::Asset* CreateAsset(std::string id) override;
	};

	class TextureAsset : public Assets::Asset
	{
	public:
		TextureAsset(std::string id);
		~TextureAsset();

		static const Core::Shortname c_assetType;
		inline const Texture* GetTexture() const { return m_renderTexture.get(); }

	private:
		bool Load(const rapidjson::Value& assetNode, const Assets::AssetDatabase& db) override;
		std::string m_textureSourcePath;
		std::unique_ptr<Texture> m_renderTexture;
	};
}