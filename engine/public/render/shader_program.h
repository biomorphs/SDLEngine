/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include <stdint.h>
#include <string>

namespace Render
{
	class ShaderBinary;

	// This represents a 'linked' set of shaders, which are used to render stuff
	class ShaderProgram
	{
	public:
		ShaderProgram();
		~ShaderProgram();

		bool Create(const ShaderBinary& vertexShader, const ShaderBinary& fragmentShader, std::string& result);
		void Destroy();

		inline uint32_t GetHandle() const { return m_handle; }

	private:
		uint32_t m_handle;
	};
}