/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "core/asset.h"
#include "core/asset_factory.h"

namespace Render
{
	class ShaderProgram;

	class ShaderProgramAssetFactory : public Core::AssetFactory
	{
	public:
		virtual ~ShaderProgramAssetFactory() { };
		std::shared_ptr<Core::Asset> CreateAsset(std::string id) override;
	};

	class ShaderProgramAsset : public Core::Asset
	{
	public:
		ShaderProgramAsset(std::string id);
		~ShaderProgramAsset();

		static const Core::Shortname c_assetType;

	private:
		bool Load(const rapidjson::Value& assetNode, const Core::AssetDatabase& db) override;
		std::string m_vertexShader;
		std::string m_fragmentShader;
		std::shared_ptr<ShaderProgram> m_program;
	};
}