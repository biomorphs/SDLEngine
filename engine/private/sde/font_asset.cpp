/*
SDLEngine
Matt Hoyle
*/
#include "font_asset.h"
#include "assets/asset.h"
#include "assets/asset_database.h"
#include "kernel/file_io.h"

namespace SDE
{
	const Core::Shortname FontAsset::c_assetType("FONT");

	struct CBFGHeader	// Header for CBFG font data
	{
		uint32_t m_mapWidth;
		uint32_t m_mapHeight;
		uint32_t m_cellWidth;
		uint32_t m_cellHeight;
		uint8_t m_startCharacter;
	};

	Assets::Asset* FontAssetFactory::CreateAsset(std::string id)
	{
		return new FontAsset(id);
	}

	FontAsset::FontAsset(std::string id)
		: Assets::Asset(id, c_assetType)
	{

	}

	FontAsset::~FontAsset()
	{

	}

	bool FontAsset::Load(const rapidjson::Value& assetNode, const Assets::AssetDatabase& db)
	{
		const auto& dataMember = assetNode.FindMember("font_data");
		SDE_ASSERT(dataMember != assetNode.MemberEnd());
		SDE_ASSERT(dataMember->value.IsString());

		const auto& textureMember = assetNode.FindMember("texture_map");
		SDE_ASSERT(textureMember != assetNode.MemberEnd());
		SDE_ASSERT(textureMember->value.IsString());

		m_textureMap = db.GetAsset(textureMember->value.GetString());
		SDE_ASSERT(m_textureMap != nullptr);

		// Load the font data from the CBFG binary file
		std::vector<uint8_t> fontRawData;
		if (!Kernel::FileIO::LoadBinaryFile(dataMember->value.GetString(), fontRawData))
		{
			SDE_LOG("Failed to open font data '%s'", dataMember->value.GetString());
			return false;
		}
		SDE_ASSERT(fontRawData.size() >= sizeof(CBFGHeader));
		CBFGHeader* headerData = reinterpret_cast<CBFGHeader*>(fontRawData.data());
		m_textureSize = glm::vec2((float)headerData->m_mapWidth, (float)headerData->m_mapHeight);
		m_cellSize = glm::vec2((float)headerData->m_cellWidth, (float)headerData->m_cellHeight);
		m_startCharacter = headerData->m_startCharacter;
		uint8_t* charWidth = &headerData->m_startCharacter + 1;
		for (uint32_t charIndex = 0;charIndex < 256; ++charIndex)
		{
			m_characterWidths.push_back(*charWidth++);
		}
		SDE_ASSERT(charWidth - fontRawData.data() <= (intptr_t)fontRawData.size());

		return true;
	}
}