/*
SDLEngine
Matt Hoyle
*/
#include "asset_serialiser.h"
#include "asset_creator.h"
#include "asset_database.h"
#include "asset.h"
#include "file_io.h"
#include "kernel/log.h"
#include "kernel/assert.h"
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <string>

namespace Core
{
	AssetSerialiser::AssetSerialiser(AssetDatabase& targetDb, AssetCreator& creator)
		: m_db(targetDb)
		, m_assetCreator(creator)
	{
	}

	AssetSerialiser::~AssetSerialiser()
	{
	}

	std::string AssetSerialiser::MakePath(const char* rootPath, const char* assetName) const
	{
		std::string assetPath = rootPath;
		assetPath += "/";
		assetPath += assetName;
		assetPath += ".asset";
		return assetPath;
	}

	bool AssetSerialiser::ProcessDependencies(const char* assetsRoot, const rapidjson::Value& depsNode)
	{
		SDE_ASSERT(depsNode.IsArray());
		for (auto dep = depsNode.Begin(); dep != depsNode.End(); ++dep)
		{
			SDE_ASSERT(dep->IsString());
			std::string dependency = dep->GetString();
			if (m_db.GetAsset(dependency).lock() == nullptr)
			{
				AssetSerialiser depSerialiser(m_db, m_assetCreator);
				if (!depSerialiser.Load(assetsRoot, dependency.c_str()))
				{
					SDE_LOGC(Core, "Failed to load dependency %s", dependency.c_str());
					return false;
				}
			}
		}
		return true;
	}

	bool AssetSerialiser::LoadAsset(const char* assetsRoot, const rapidjson::Value& assetNode)
	{
		auto nameMember = assetNode.FindMember("id");
		SDE_ASSERT(nameMember != assetNode.MemberEnd() && nameMember->value.IsString());
		auto typeMember = assetNode.FindMember("typeid");
		SDE_ASSERT(typeMember != assetNode.MemberEnd() && typeMember->value.IsString());
		auto dataMember = assetNode.FindMember("data");
		SDE_ASSERT(dataMember != assetNode.MemberEnd() && dataMember->value.IsObject());
		if (nameMember == assetNode.MemberEnd() || typeMember == assetNode.MemberEnd() || dataMember == assetNode.MemberEnd())
		{
			SDE_LOGC(Core, "Missing member in asset definition");
			return false;
		}
		// early-out if its in the db already
		std::string assetName = nameMember->value.GetString();
		if (m_db.GetAsset(assetName).lock() != nullptr)
		{
			SDE_LOGC(Core, "Asset %s already loaded", assetName.c_str());
			return false;
		}

		auto dependenciesMember = assetNode.FindMember("dependencies");
		if (dependenciesMember != assetNode.MemberEnd() && !ProcessDependencies(assetsRoot, dependenciesMember->value))
		{
			SDE_LOGC(Core, "Missing dependencies in asset %s", assetName.c_str());
			return false;
		}

		auto createdAsset = m_assetCreator.CreateAsset(assetName, Core::Shortname(typeMember->value.GetString()));
		if (createdAsset)
		{
			if (!createdAsset->Load(dataMember->value, m_db))
			{
				SDE_LOGC(Core, "Failed to load asset of type '%s'", typeMember->value.GetString());
				return false;
			}
			m_db.AddAsset(createdAsset);
			m_loadedAssets.push_back(assetName);
		}
		else
		{
			SDE_LOGC(Core, "Failed to create asset of type '%s'", typeMember->value.GetString());
			return false;
		}
		return true;
	}

	bool AssetSerialiser::Load(const char* assetsRoot, const char* jsonPath)
	{
		// First, load the data into one big fat string
		std::string rawJsonData;
		std::string assetPath = MakePath(assetsRoot, jsonPath);
		if (!LoadTextFromFile(assetPath.c_str(), rawJsonData))
		{
			SDE_LOGC(Core, "Failed to open file '%s'", jsonPath);
			return false;
		}		

		// Parse the JSON DOM
		rapidjson::Document theDocument;
		theDocument.Parse(rawJsonData.c_str());
		if (theDocument.HasParseError())
		{
			SDE_LOGC(Core, "Failed to parse json document: %s, offset %d", rapidjson::GetParseError_En(theDocument.GetParseError()), theDocument.GetErrorOffset());
			return false;
		}

		// Now iterate through the DOM tree, pulling out any asset definitions
		for (auto docNode = theDocument.MemberBegin(); docNode != theDocument.MemberEnd(); ++docNode)
		{
			SDE_ASSERT(docNode->value.IsObject());
			SDE_ASSERT(docNode->name.IsString());
			if (!LoadAsset(assetsRoot, docNode->value))
			{
				return false;
			}
		}

		return true;
	}
}