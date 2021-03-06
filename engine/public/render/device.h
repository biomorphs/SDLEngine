/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "kernel/base_types.h"
#include <glm/glm.hpp>

namespace Render
{
	class Window;
	class Texture;
	class VertexArray;
	class ShaderProgram;

	enum class PrimitiveType : uint32_t
	{
		Triangles,
		Lines,
		PointSprites
	};

	// This represents the GL context for a window
	class Device
	{
	public:
		Device(Window& theWindow);
		~Device();
		void Present();
		void* GetGLContext();
		void SetScissorEnabled(bool enabled);
		void SetBlending(bool enabled);
		void SetBackfaceCulling(bool enabled, bool frontFaceCCW);
		void SetDepthState(bool enabled, bool writeEnabled);
		void ClearColourDepthTarget(const glm::vec4& colour);
		void SetUniformValue(uint32_t uniformHandle, const glm::mat4& matrix);
		void SetUniformValue(uint32_t uniformHandle, const glm::vec4& val);
		void SetSampler(uint32_t uniformHandle, uint32_t textureHandle, uint32_t textureUnit);
		void SetArraySampler(uint32_t uniformHandle, uint32_t textureHandle, uint32_t textureUnit);
		void BindShaderProgram(const ShaderProgram& program);
		void BindVertexArray(const VertexArray& srcArray);
		void DrawPrimitives(PrimitiveType primitive, uint32_t vertexStart, uint32_t vertexCount);
	private:
		uint32_t TranslatePrimitiveType(PrimitiveType type) const;

		Window& m_window;
		void* m_context;
	};
}