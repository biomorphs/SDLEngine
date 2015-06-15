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

	private:
		ShaderProgram* m_shader;
	};
}