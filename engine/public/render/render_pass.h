/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "camera.h"
#include "instance_queue.h"

namespace Render
{
	class Device;
	class ShaderProgram;
	class Material;
	class UniformBuffer;

	struct RenderState
	{
		RenderState()
			: m_depthTestEnabled(true)
			, m_depthWritingEnabled(true)
			, m_backfaceCullEnabled(true)
			, m_frontFaceOrderCCW(true)
			, m_blendingEnabled(false) 
			, m_scissorEnabled(false) {}
		bool m_depthTestEnabled;
		bool m_depthWritingEnabled;
		bool m_backfaceCullEnabled;
		bool m_frontFaceOrderCCW;
		bool m_blendingEnabled;
		bool m_scissorEnabled;
	};

	// Contains everything required to draw a set of instances in one pass
	class RenderPass
	{
	public:
		RenderPass();
		~RenderPass();

		inline RenderState& GetRenderState() { return m_renderState; }

		void AddInstance(const Mesh* mesh);
		void AddInstance(const Mesh* mesh, UniformBuffer&& instanceUniforms);
		void AddInstance(const Mesh* mesh, UniformBuffer&& instanceUniforms, uint32_t startChunk, uint32_t endChunk);
		void Reset();
		void RenderAll(Device& device);

	private:
		void ApplyRenderState(Device& d);
		void ApplyUniforms(Device& d, const ShaderProgram& p, const UniformBuffer& uniforms);

		RenderState m_renderState;
		InstanceQueue m_instances;
	};
}