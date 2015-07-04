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

	struct RenderState
	{
		RenderState()
			: m_depthTestEnabled(true), m_depthWritingEnabled(true), m_backfaceCullEnabled(true), m_frontFaceOrderCCW(true) {}
		bool m_depthTestEnabled;
		bool m_depthWritingEnabled;
		bool m_backfaceCullEnabled;
		bool m_frontFaceOrderCCW;
	};

	// Contains everything required to draw a set of instances in one pass
	class RenderPass
	{
	public:
		RenderPass();
		~RenderPass();

		inline Camera& GetCamera()	{ return m_camera; }
		inline RenderState& GetRenderState() { return m_renderState; }

		inline void AddInstance(const Mesh* mesh, const glm::mat4& transform)	{ m_instances.AddInstance(mesh, transform); }
		void Reset();
		void RenderAll(Device& device);

	private:
		void ApplyRenderState(Device& d);

		RenderState m_renderState;
		Camera m_camera;
		InstanceQueue m_instances;
	};
}