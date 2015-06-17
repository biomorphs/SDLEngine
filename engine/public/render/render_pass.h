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

	// Contains everything required to draw a set of instances in one pass
	class RenderPass
	{
	public:
		RenderPass();
		~RenderPass();

		inline void AddInstance(std::shared_ptr<Mesh>& mesh, const glm::mat4& transform)	{ m_instances.AddInstance(mesh, transform); }
		void Reset();
		void RenderAll(Device& device);

	private:
		Camera m_camera;
		InstanceQueue m_instances;
	};
}