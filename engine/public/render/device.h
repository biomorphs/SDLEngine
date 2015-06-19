/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include <stdint.h>
#include <SDL.h>
#include <glm.hpp>

struct SDL_Renderer;

namespace Render
{
	class Window;
	class Texture;
	class VertexArray;
	class ShaderProgram;

	enum class PrimitiveType : uint32_t
	{
		Triangles
	};

	// This represents the GL context for a window
	class Device
	{
	public:
		Device(Window& theWindow);
		~Device();
		void Present();
		SDL_GLContext GetGLContext();

		void ClearColourDepthTarget(const glm::vec4& colour);
		void SetUniformValue(uint32_t uniformHandle, const glm::mat4& matrix);
		void BindShaderProgram(const ShaderProgram& program);
		void BindVertexArray(const VertexArray& srcArray);
		void DrawPrimitives(PrimitiveType primitive, uint32_t vertexStart, uint32_t vertexCount);
	private:
		uint32_t TranslatePrimitiveType(PrimitiveType type) const;

		Window& m_window;
		SDL_GLContext m_context;
	};
}