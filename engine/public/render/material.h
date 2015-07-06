/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "kernel/base_types.h"
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
			GlobalParameterDefinitions()
				: m_mvpUniformHandle(-1)
			{
			}
			uint32_t m_mvpUniformHandle;
		};

		inline void SetShaderProgram(const ShaderProgram* program)				{ m_shader = program; }
		inline const ShaderProgram* GetShaderProgram() const					{ return m_shader; }
		GlobalParameterDefinitions& GlobalDefinitions()							{ return m_globalDefinitions; }
		const GlobalParameterDefinitions& GlobalDefinitions() const				{ return m_globalDefinitions; }

	private:
		const ShaderProgram* m_shader;
		GlobalParameterDefinitions m_globalDefinitions;
	};
}