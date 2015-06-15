/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include <stdint.h>
#include <SDL.h>

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
		void BindShaderProgram(const ShaderProgram& program);
		void DrawArray(const VertexArray& srcArray, PrimitiveType primitive, uint32_t vertexStart, uint32_t vertexCount);
	private:
		uint32_t TranslatePrimitiveType(PrimitiveType type) const;

		Window& m_window;
		SDL_GLContext m_context;
	};
}