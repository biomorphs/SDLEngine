/*
SDLEngine
Matt Hoyle
*/
#pragma once
#include "texture_asset.h"
#include "texture.h"
#include "dds_loader.h"
#include "texture_source.h"

namespace Render
{
	const Core::Shortname TextureAsset::c_assetType("RTEX");

	Assets::Asset* TextureAssetFactory::CreateAsset(std::string id)
	{
		return new TextureAsset(id);
	}

	TextureAsset::TextureAsset(std::string id)
		: Assets::Asset(id, c_assetType)
	{
	}

	TextureAsset::~TextureAsset()
	{
	}

	bool TextureAsset::Load(const rapidjson::Value& assetNode, const Assets::AssetDatabase& db)
	{
		auto sourcePathMember = assetNode.FindMember("source_textures");
		SDE_ASSERT(sourcePathMember != assetNode.MemberEnd());
		SDE_ASSERT(sourcePathMember->value.IsArray());

		DDSLoader imageLoader;
		std::vector<TextureSource> loadedTextures;
		for (auto tex = sourcePathMember->value.Begin();
			tex != sourcePathMember->value.End();
			++tex)
		{
			// load each texture source (this may be a texture array)
			TextureSource src;
			bool loadedOK = imageLoader.LoadFile(tex->GetString(), src);
			if (!loadedOK)
			{
				SDE_LOGC(Render, "Failed to load texture asset '%s'", tex->GetString());
				return false;
			}
			loadedTextures.push_back(std::move(src));
		}

		m_renderTexture = std::make_unique<Texture>();
		if (!m_renderTexture->Create(loadedTextures))
		{
			m_renderTexture = nullptr;
			SDE_LOGC(Render, "Failed to create texture from source data");
			return false;
		}

		return true;
	}
}