/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "shortname.h"
#include <rapidjson\document.h>

namespace Core
{
	// Assets are wrappers around *constant* data objects that will be shared
	// Things like textures, materials, meshes, etc that are loaded externally
	class AssetDatabase;
	class Asset
	{
	public:
		Asset(std::string id, Shortname assetTypeName);
		virtual ~Asset();

		virtual bool Load(const rapidjson::Value& assetNode, const AssetDatabase& db) = 0;
		inline const std::string& GetID() const { return m_assetIdStr; }

	private:
		std::string m_assetIdStr;
		Shortname m_assetType;
	};
}