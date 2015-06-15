/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include <stdint.h>
#include <string>

namespace Render
{
	enum class ShaderType : uint32_t
	{
		VertexShader,
		FragmentShader
	};

	// This represents a single compiled shader. It can be vertex, fragment, hull, whatever
	// Shaders must be linked in a ShaderProgram to be usable for rendering
	class ShaderBinary
	{
	public:
		ShaderBinary();
		~ShaderBinary();

		bool CompileFromFile(ShaderType type, const char* srcLocation, std::string& resultText);
		void Destroy();

		inline uint32_t GetHandle() const { return m_handle; }
		inline ShaderType GetType() const { return m_type; }

	private:
		bool CompileSource(ShaderType type, const std::string& src, std::string& resultText);
		uint32_t TranslateShaderType(ShaderType type) const;

		uint32_t m_handle;
		ShaderType m_type;
	};
}