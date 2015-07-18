/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "uniform_buffer.h"
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

		inline void SetShaderProgram(const ShaderProgram* program)	{ m_shader = program; }
		inline const ShaderProgram* GetShaderProgram() const		{ return m_shader; }
		inline UniformBuffer& GetUniforms()							{ return m_uniforms; }
		inline const UniformBuffer& GetUniforms() const				{ return m_uniforms; }

	private:
		const ShaderProgram* m_shader;
		UniformBuffer m_uniforms;
	};
}