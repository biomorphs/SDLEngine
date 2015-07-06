/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "kernel/base_types.h"
#include <glm.hpp>

namespace Render
{
	class Window;
	class Texture;
	class VertexArray;
	class ShaderProgram;

	enum class PrimitiveType : uint32_t
	{
		Triangles,
		Lines
	};

	// This represents the GL context for a window
	class Device
	{
	public:
		Device(Window& theWindow);
		~Device();
		void Present();
		void* GetGLContext();

		void SetBackfaceCulling(bool enabled, bool frontFaceCCW);
		void SetDepthState(bool enabled, bool writeEnabled);
		void ClearColourDepthTarget(const glm::vec4& colour);
		void SetUniformValue(uint32_t uniformHandle, const glm::mat4& matrix);
		void BindShaderProgram(const ShaderProgram& program);
		void BindVertexArray(const VertexArray& srcArray);
		void DrawPrimitives(PrimitiveType primitive, uint32_t vertexStart, uint32_t vertexCount);
	private:
		uint32_t TranslatePrimitiveType(PrimitiveType type) const;

		Window& m_window;
		void* m_context;
	};
}