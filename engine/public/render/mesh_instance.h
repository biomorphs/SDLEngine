/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "uniform_buffer.h"
#include <memory>
#include <glm/glm.hpp>

namespace Render
{
	class Mesh;

	// This contains everything used to render a mesh
	class MeshInstance
	{
	public:
		MeshInstance() { }
		explicit MeshInstance(const Mesh* mesh, uint32_t startChunk, uint32_t endChunk)
			: m_mesh(mesh)
			, m_startChunk(startChunk)
			, m_endChunk(endChunk)
		{
		}
		explicit MeshInstance(const Mesh* mesh, uint32_t startChunk, uint32_t endChunk, UniformBuffer&& uniforms)
			: m_mesh(mesh)
			, m_startChunk(startChunk)
			, m_endChunk(endChunk)
			, m_instanceUniforms(std::move(uniforms))
		{
		}
		~MeshInstance() { }

		inline const Mesh* GetMesh() const				{ return m_mesh; }	
		inline const UniformBuffer& GetUniforms() const { return m_instanceUniforms; }
		uint32_t GetStartChunk() const					{ return m_startChunk; }
		uint32_t GetEndChunk() const					{ return m_endChunk; }

	private:
		const Mesh* m_mesh;
		UniformBuffer m_instanceUniforms;
		uint32_t m_startChunk;
		uint32_t m_endChunk;
	};
}