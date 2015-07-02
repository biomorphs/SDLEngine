/*
SDLEngine
Matt Hoyle
*/
#include "asset.h"

namespace Core
{
	Asset::Asset(std::string id, Shortname assetTypeName)
		: m_assetIdStr(id)
		, m_assetType(assetTypeName)
	{
	}

	Asset::~Asset()
	{
	}
}