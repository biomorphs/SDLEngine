/*
SDLEngine
Matt Hoyle
*/

#include "device.h"
#include "window.h"
#include "utils.h"
#include "vertex_array.h"
#include "shader_program.h"
#include <SDL.h>
#include <glew.h>
#include <gtc/type_ptr.hpp>

namespace Render
{
	Device::Device(Window& theWindow)
		: m_window( theWindow )
	{
		SDL_Window* windowHandle = theWindow.GetWindowHandle();
		SDE_RENDER_ASSERT(windowHandle);

		// Request opengl 3.0 context.
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

		// Always doublebuffer + 24 bit depth
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		m_context = SDL_GL_CreateContext(windowHandle);
		SDE_RENDER_ASSERT(m_context);

		SDL_GL_SetSwapInterval(1);	// 0 = no vsync, 1 = vsync

		// glew initialises GL function pointers
		glewExperimental = true;		// must be set for core profile and above
		auto glewError = glewInit();
		SDE_ASSERT(glewError == GLEW_OK, "Failed to initialise glew: %s", glewGetErrorString(glewError));

		// glewInit can push INVALID_ENUM to the error stack (due to how extensions are checked)
		// pop everything off here
		auto glErrorPop = glGetError();
		while (glErrorPop == GL_INVALID_ENUM)
		{
			glErrorPop = glGetError();
		}
		SDE_RENDER_PROCESS_GL_ERRORS("Device Initialise");
	}

	Device::~Device()
	{
		SDL_GL_DeleteContext(m_context);
		m_context = nullptr;
	}

	void Device::Present()
	{
		SDL_GL_SwapWindow(m_window.GetWindowHandle());
	}

	SDL_GLContext Device::GetGLContext()
	{
		return m_context;
	}

	inline uint32_t Device::TranslatePrimitiveType(PrimitiveType type) const
	{
		switch (type)
		{
		case PrimitiveType::Triangles:
			return GL_TRIANGLES;
		case PrimitiveType::Lines:
			return GL_LINES;
		default:
			return -1;
		}
	}

	void Device::SetBackfaceCulling(bool enabled, bool frontFaceCCW)
	{
		if (enabled)
		{
			glEnable(GL_CULL_FACE);
			SDE_RENDER_PROCESS_GL_ERRORS("glEnable");
		}
		else
		{
			glDisable(GL_CULL_FACE);
			SDE_RENDER_PROCESS_GL_ERRORS("glDisable");
		}

		glFrontFace(frontFaceCCW ? GL_CCW : GL_CW);
		SDE_RENDER_PROCESS_GL_ERRORS("glFrontFace");
	}

	void Device::SetDepthState(bool enabled, bool writeEnabled)
	{
		if (enabled)
		{
			glEnable(GL_DEPTH_TEST);
			SDE_RENDER_PROCESS_GL_ERRORS("glEnable");
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
			SDE_RENDER_PROCESS_GL_ERRORS("glDisable");
		}
		glDepthMask(writeEnabled);
		SDE_RENDER_PROCESS_GL_ERRORS("glDepthMask");
	}

	void Device::ClearColourDepthTarget(const glm::vec4& colour)
	{
		glClearColor(colour.r, colour.g, colour.b, colour.a);
		SDE_RENDER_PROCESS_GL_ERRORS("glClearColor");
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		SDE_RENDER_PROCESS_GL_ERRORS("glClear");
	}

	void Device::SetUniformValue(uint32_t uniformHandle, const glm::mat4& matrix)
	{
		SDE_ASSERT(uniformHandle != -1);
		glUniformMatrix4fv(uniformHandle, 1, GL_FALSE, glm::value_ptr(matrix));
		SDE_RENDER_PROCESS_GL_ERRORS("glUniformMatrix4fv");
	}

	void Device::BindShaderProgram(const ShaderProgram& program)
	{
		glUseProgram(program.GetHandle());
		SDE_RENDER_PROCESS_GL_ERRORS("glUseProgram");
	}

	void Device::BindVertexArray(const VertexArray& srcArray)
	{
		SDE_ASSERT(srcArray.GetHandle() != 0);
		glBindVertexArray(srcArray.GetHandle());
		SDE_RENDER_PROCESS_GL_ERRORS("glBindVertexArray");
	}

	void Device::DrawPrimitives(PrimitiveType primitive, uint32_t vertexStart, uint32_t vertexCount)
	{
		auto primitiveType = TranslatePrimitiveType(primitive);
		SDE_ASSERT(primitiveType != -1);

		glDrawArrays(primitiveType, vertexStart, vertexCount);
		SDE_RENDER_PROCESS_GL_ERRORS("glDrawArrays");
	}
}