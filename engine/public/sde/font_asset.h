/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "assets/asset.h"
#include "assets/asset_factory.h"
#include <vector>
#include <glm.hpp>

namespace SDE
{
	class FontAssetFactory : public Assets::AssetFactory
	{
	public:
		virtual ~FontAssetFactory() { };
		Assets::Asset* CreateAsset(std::string id) override;
	};

	class FontAsset : public Assets::Asset
	{
	public:
		FontAsset(std::string id);
		~FontAsset();

		static const Core::Shortname c_assetType;

	private:
		bool Load(const rapidjson::Value& assetNode, const Assets::AssetDatabase& db) override;
		std::shared_ptr<Assets::Asset> m_textureMap;
		glm::vec2 m_textureSize;
		glm::vec2 m_cellSize;
		uint8_t m_startCharacter;
		std::vector<float> m_characterWidths;
	};
}