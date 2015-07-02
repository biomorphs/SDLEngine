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

	std::shared_ptr<Core::Asset> ShaderProgramAssetFactory::CreateAsset(std::string id)
	{
		auto theAsset = std::make_shared<ShaderProgramAsset>(id);
		return theAsset;
	}

	ShaderProgramAsset::ShaderProgramAsset(std::string id)
		: Core::Asset(id, c_assetType)
	{
	}

	ShaderProgramAsset::~ShaderProgramAsset()
	{
	}

	bool ShaderProgramAsset::Load(const rapidjson::Value& assetNode, const Core::AssetDatabase& db)
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

		m_program = std::make_shared<Render::ShaderProgram>();
		if (!m_program->Create(vertexShader, fragmentShader, compileResult))
		{
			SDE_LOG("Failed to link shaders: \r\n\t%s", compileResult.c_str());
			return false;
		}

		return true;
	}
}