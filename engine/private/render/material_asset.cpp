/*
SDLEngine
Matt Hoyle
*/
#pragma once
#include "material_asset.h"
#include "material.h"
#include "shader_program_asset.h"
#include "texture_asset.h"
#include "assets/asset_database.h"
#include <glm.hpp>

namespace Render
{
	const Core::Shortname MaterialAsset::c_assetType("RMAT");

	Assets::Asset* MaterialAssetFactory::CreateAsset(std::string id)
	{
		return new MaterialAsset(id);
	}

	MaterialAsset::MaterialAsset(std::string id)
		: Assets::Asset(id, c_assetType)
	{
	}

	MaterialAsset::~MaterialAsset()
	{
	}

	void ParseVec4(const char* buffer, glm::vec4& result)
	{
		sscanf_s(buffer, "%f,%f,%f,%f", &result.x, &result.y, &result.z, &result.w);
	}

	bool MaterialAsset::Load(const rapidjson::Value& assetNode, const Assets::AssetDatabase& db)
	{
		auto shaderProgramMember = assetNode.FindMember("shader_program");
		SDE_ASSERT(shaderProgramMember != assetNode.MemberEnd());
		m_shaderProgramId = shaderProgramMember->value.GetString();

		// get the shader program, we will take a shared_ptr since we want to enforce its lifetime
		m_shaderProgramAsset = db.GetAsset(m_shaderProgramId);
		SDE_ASSERT(m_shaderProgramAsset != nullptr);

		// set up the render material
		ShaderProgramAsset* program = static_cast<ShaderProgramAsset*>(m_shaderProgramAsset.get());
		m_renderMaterial = std::make_unique<Material>();
		m_renderMaterial->SetShaderProgram(program->GetShaderProgram());

		// Parse the uniforms
		auto& uniformsVec4 = assetNode.FindMember("uniforms_vec4");
		if (uniformsVec4 != assetNode.MemberEnd())
		{
			SDE_ASSERT(uniformsVec4->value.IsObject());
			for (auto uni = uniformsVec4->value.MemberBegin();
				uni != uniformsVec4->value.MemberEnd();
				++uni)
			{
				const char* name = uni->name.GetString();
				const char* valueStr = uni->value.GetString();
				glm::vec4 value(0.0f);
				ParseVec4(valueStr, value);
				m_renderMaterial->GetUniforms().SetValue(name, value);
			}
		}

		auto& uniformsTextures = assetNode.FindMember("uniforms_textures");
		if (uniformsTextures != assetNode.MemberEnd())
		{
			SDE_ASSERT(uniformsTextures->value.IsObject());
			for (auto uni = uniformsTextures->value.MemberBegin();
			uni != uniformsTextures->value.MemberEnd();
				++uni)
			{
				const char* samplerName = uni->name.GetString();
				auto theTexture = db.GetAsset(uni->value.GetString());
				SDE_ASSERT(theTexture != nullptr);
				auto textureAsset = static_cast<TextureAsset*>(theTexture.get());
				m_renderMaterial->GetUniforms().SetValue(samplerName, textureAsset->GetTexture());
			}
		}

		return true;
	}
}