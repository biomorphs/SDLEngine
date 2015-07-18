/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "assets/asset.h"
#include "assets/asset_factory.h"

namespace Render
{
	class ShaderProgram;

	class ShaderProgramAssetFactory : public Assets::AssetFactory
	{
	public:
		virtual ~ShaderProgramAssetFactory() { };
		Assets::Asset* CreateAsset(std::string id) override;
	};

	class ShaderProgramAsset : public Assets::Asset
	{
	public:
		ShaderProgramAsset(std::string id);
		~ShaderProgramAsset();

		inline const ShaderProgram* GetShaderProgram() const { return m_program.get(); }

		static const Core::Shortname c_assetType;

	private:
		bool Load(const rapidjson::Value& assetNode, const Assets::AssetDatabase& db) override;
		std::string m_vertexShader;
		std::string m_fragmentShader;
		std::unique_ptr<ShaderProgram> m_program;
	};
}