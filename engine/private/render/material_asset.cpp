/*
SDLEngine
Matt Hoyle
*/
#pragma once
#include "material_asset.h"
#include "material.h"
#include "core/asset_database.h"

namespace Render
{
	const Core::Shortname MaterialAsset::c_assetType("RMAT");

	std::shared_ptr<Core::Asset> MaterialAssetFactory::CreateAsset(std::string id)
	{
		auto theAsset = std::make_shared<MaterialAsset>(id);
		return theAsset;
	}

	MaterialAsset::MaterialAsset(std::string id)
		: Core::Asset(id, c_assetType)
	{
	}

	MaterialAsset::~MaterialAsset()
	{
	}

	bool MaterialAsset::Load(const rapidjson::Value& assetNode, const Core::AssetDatabase& db)
	{
		auto shaderProgramMember = assetNode.FindMember("shader_program");
		SDE_ASSERT(shaderProgramMember != assetNode.MemberEnd());
		m_shaderProgramId = shaderProgramMember->value.GetString();

		m_shaderProgramAsset = db.GetAsset(m_shaderProgramId);
		SDE_ASSERT(m_shaderProgramAsset.lock() != nullptr);
		
		return true;
	}
}