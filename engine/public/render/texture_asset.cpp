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
		auto sourcePathMember = assetNode.FindMember("source_texture");
		SDE_ASSERT(sourcePathMember != assetNode.MemberEnd());
		SDE_ASSERT(sourcePathMember->value.IsString());

		m_textureSourcePath = sourcePathMember->value.GetString();
		DDSLoader imageLoader;
		TextureSource sourceData;
		bool loadedOK = imageLoader.LoadFile(m_textureSourcePath.c_str(), sourceData);
		if (!loadedOK)
		{
			SDE_LOGC(Render, "Failed to load texture asset '%s'", m_textureSourcePath.c_str());
			return false;
		}

		m_renderTexture = std::make_unique<Texture>();
		if (!m_renderTexture->Create(sourceData))
		{
			m_renderTexture = nullptr;
			SDE_LOGC(Render, "Failed to create texture from source data");
			return false;
		}

		return true;
	}
}