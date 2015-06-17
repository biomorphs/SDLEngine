/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "mesh_instance.h"
#include <vector>

namespace Render
{
	// This contains a queue of mesh instances to be rendered
	class InstanceQueue
	{
	public:
		InstanceQueue(uint32_t reservedSize = 1024);
		~InstanceQueue();

		inline void AddInstance(std::shared_ptr<Mesh>& mesh, const glm::mat4& transform)
		{
			m_instances.emplace_back(mesh, transform);
		}

		inline void Clear()
		{
			m_instances.clear();
		}

		inline std::vector<MeshInstance>::const_iterator begin() const
		{
			return m_instances.begin();
		}
		
		inline std::vector<MeshInstance>::const_iterator end() const
		{
			return m_instances.end();
		}

	private:
		std::vector<MeshInstance> m_instances;
	};
}