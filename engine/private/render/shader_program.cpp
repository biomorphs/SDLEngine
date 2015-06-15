/*
SDLEngine
Matt Hoyle
*/
#include "shader_program.h"
#include "shader_binary.h"
#include "utils.h"

namespace Render
{
	ShaderProgram::ShaderProgram()
		: m_handle(0)
	{
	}

	ShaderProgram::~ShaderProgram()
	{
		SDE_ASSERT(m_handle == 0, "ShaderProgram leaked");
	}

	bool ShaderProgram::Create(const ShaderBinary& vertexShader, const ShaderBinary& fragmentShader, std::string& result)
	{
		SDE_ASSERT(vertexShader.GetType() == ShaderType::VertexShader);
		SDE_ASSERT(fragmentShader.GetType() == ShaderType::FragmentShader);
		SDE_ASSERT(vertexShader.GetHandle() != 0);
		SDE_ASSERT(fragmentShader.GetHandle() != 0);

		m_handle = glCreateProgram();
		SDE_RENDER_PROCESS_GL_ERRORS_RET("glCreateProgram");

		glAttachShader(m_handle, vertexShader.GetHandle());
		SDE_RENDER_PROCESS_GL_ERRORS_RET("glAttachShader");

		glAttachShader(m_handle, fragmentShader.GetHandle());
		SDE_RENDER_PROCESS_GL_ERRORS_RET("glAttachShader");

		glLinkProgram(m_handle);
		SDE_RENDER_PROCESS_GL_ERRORS_RET("glLinkProgram");

		// check the results
		int32_t linkResult = 0, logLength = 0;
		char resultTxt[1024] = { '\0' };
		glGetProgramiv(m_handle, GL_LINK_STATUS, &linkResult);
		glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &logLength);

		SDE_ASSERT(logLength < sizeof(resultTxt));
		glGetProgramInfoLog(m_handle, logLength, NULL, resultTxt);
		result = resultTxt;

		return linkResult == GL_TRUE;
	}

	void ShaderProgram::Destroy()
	{
		glDeleteProgram(m_handle);
		SDE_RENDER_PROCESS_GL_ERRORS("glDeleteProgram");
		m_handle = 0;
	}
}
