/*
SDLEngine
Matt Hoyle
*/
#pragma once

namespace Render
{
	class ShaderProgram;

	class Material
	{
	public:
		Material();
		~Material();

		inline void SetShaderProgram(ShaderProgram* program)	{ m_shader = program; }
		inline ShaderProgram* GetShaderProgram() const { return m_shader; }

	private:
		ShaderProgram* m_shader;
	};
}