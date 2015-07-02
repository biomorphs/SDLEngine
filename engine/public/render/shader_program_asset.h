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
		Core::Asset* CreateAsset(std::string id) override;
	};

	class ShaderProgramAsset : public Core::Asset
	{
	public:
		ShaderProgramAsset(std::string id);
		~ShaderProgramAsset();

		inline const ShaderProgram* GetShaderProgram() const { return m_program.get(); }

		// Global param uniform names are stored in the asset since they are tied to the shaders
		inline const std::string& GetMVPUniformName() const { return m_mvpUniformName; }

		static const Core::Shortname c_assetType;

	private:
		bool Load(const rapidjson::Value& assetNode, const Core::AssetDatabase& db) override;
		std::string m_vertexShader;
		std::string m_fragmentShader;
		std::string m_mvpUniformName;
		std::unique_ptr<ShaderProgram> m_program;
	};
}