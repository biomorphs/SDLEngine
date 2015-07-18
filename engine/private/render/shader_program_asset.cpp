/*
SDLEngine
Matt Hoyle
*/
#pragma once
#include "shader_program_asset.h"
#include "shader_program.h"
#include "shader_binary.h"

namespace Render
{
	const Core::Shortname ShaderProgramAsset::c_assetType("RSHP");

	Assets::Asset* ShaderProgramAssetFactory::CreateAsset(std::string id)
	{
		return new ShaderProgramAsset(id);
	}

	ShaderProgramAsset::ShaderProgramAsset(std::string id)
		: Assets::Asset(id, c_assetType)
	{
	}

	ShaderProgramAsset::~ShaderProgramAsset()
	{
	}

	bool ShaderProgramAsset::Load(const rapidjson::Value& assetNode, const Assets::AssetDatabase& db)
	{
		auto vsMember = assetNode.FindMember("vertexshader");
		auto fsMember = assetNode.FindMember("fragmentshader");
		SDE_ASSERT(vsMember != assetNode.MemberEnd());
		SDE_ASSERT(fsMember != assetNode.MemberEnd());
		m_vertexShader = vsMember->value.GetString();
		m_fragmentShader = fsMember->value.GetString();

		std::string compileResult;
		ShaderBinary vertexShader, fragmentShader;
		if (!vertexShader.CompileFromFile(Render::ShaderType::VertexShader, m_vertexShader.c_str(), compileResult))
		{
			SDE_LOG("Failed to compile shader:\r\n\t%s", compileResult.c_str());
			return false;
		}
		if (!fragmentShader.CompileFromFile(Render::ShaderType::FragmentShader, m_fragmentShader.c_str(), compileResult))
		{
			SDE_LOG("Failed to compile shader:\r\n\t%s", compileResult.c_str());
			return false;
		}

		m_program = std::make_unique<Render::ShaderProgram>();
		if (!m_program->Create(vertexShader, fragmentShader, compileResult))
		{
			SDE_LOG("Failed to link shaders: \r\n\t%s", compileResult.c_str());
			return false;
		}

		// load any parameter names
		auto uniformsObject = assetNode.FindMember("uniforms");
		SDE_ASSERT(uniformsObject != assetNode.MemberEnd() && uniformsObject->value.IsArray());
		for (auto docNode = uniformsObject->value.Begin();
			 docNode != uniformsObject->value.End();
			 ++docNode)
		{
			const char* uniformName = docNode->GetString();
			m_program->AddUniform(uniformName);
		}

		return true;
	}
}