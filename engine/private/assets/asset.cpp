/*
SDLEngine
Matt Hoyle
*/
#include "asset.h"

namespace Assets
{
	Asset::Asset(const std::string& id, Core::Shortname assetTypeName)
		: m_assetIdStr(id)
		, m_assetType(assetTypeName)
	{
	}

	Asset::~Asset()
	{
	}
}