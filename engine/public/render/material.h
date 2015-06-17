/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include <stdint.h>
#include <memory>

namespace Render
{
	class ShaderProgram;

	class Material
	{
	public:
		Material();
		~Material();

		// This struct contains uniform handles for data passed to *all* shaders
		struct GlobalParameterDefinitions
		{
			uint32_t m_mvpUniformHandle;
		};

		inline void SetShaderProgram(std::shared_ptr<ShaderProgram>& program)	{ m_shader = program; }
		inline const ShaderProgram* GetShaderProgram() const					{ return m_shader.get(); }
		GlobalParameterDefinitions& GlobalDefinitions()							{ return m_globalDefinitions; }
		const GlobalParameterDefinitions& GlobalDefinitions() const				{ return m_globalDefinitions; }

	private:
		std::shared_ptr<ShaderProgram> m_shader;
		GlobalParameterDefinitions m_globalDefinitions;
	};
}