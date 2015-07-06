/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "kernel/base_types.h"
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

		uint32_t GetUniformHandle(const char* uniformName) const;
		inline uint32_t GetHandle() const { return m_handle; }

	private:
		uint32_t m_handle;
	};
}