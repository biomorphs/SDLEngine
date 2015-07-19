/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "mesh_instance.h"
#include <vector>

namespace Render
{
	class UniformBuffer;

	// This contains a queue of mesh instances to be rendered
	class InstanceQueue
	{
	public:
		InstanceQueue(uint32_t reservedSize = 1024)
		{
			m_instances.reserve(reservedSize);
		}
		~InstanceQueue()	{ }

		inline void AddInstance(const Mesh* mesh, uint32_t startChunk, uint32_t endChunk)
		{
			m_instances.emplace_back(mesh, startChunk, endChunk);
		}

		inline void AddInstance(const Mesh* mesh, uint32_t startChunk, uint32_t endChunk, UniformBuffer&& uniforms)
		{
			m_instances.emplace_back(mesh, startChunk, endChunk, std::move(uniforms));
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