/*
SDLEngine
Matt Hoyle
*/
#pragma once
#include "material_asset.h"
#include "material.h"
#include "shader_program_asset.h"
#include "shader_program.h"
#include "core/asset_database.h"

namespace Render
{
	const Core::Shortname MaterialAsset::c_assetType("RMAT");

	Core::Asset* MaterialAssetFactory::CreateAsset(std::string id)
	{
		return new MaterialAsset(id);
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

		// get the shader program, we will take a shared_ptr since we want to enforce its lifetime
		m_shaderProgramAsset = db.GetAsset(m_shaderProgramId);
		SDE_ASSERT(m_shaderProgramAsset != nullptr);

		// the render material needs to know the shader param IDs for global data
		// it is stored in the shader program asset
		ShaderProgramAsset* program = static_cast<ShaderProgramAsset*>(m_shaderProgramAsset.get());
		const std::string& mvpUniformName = program->GetMVPUniformName();
		uint32_t mvpHandle = program->GetShaderProgram()->GetUniformHandle(mvpUniformName.c_str());
		
		// set up the render material
		m_renderMaterial = std::make_unique<Material>();
		m_renderMaterial->SetShaderProgram(program->GetShaderProgram());
		m_renderMaterial->GlobalDefinitions().m_mvpUniformHandle = mvpHandle;

		return true;
	}
}