/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include <stdint.h>
#include <glm.hpp>
#include "mesh.h"

namespace Render
{
	// Helper for creating mesh objects
	class MeshBuilder
	{
	public:
		MeshBuilder();
		~MeshBuilder();

		// Step 1: Define streams
		uint32_t AddVertexStream(int32_t componentCount);

		// Step 2: Define chunks
		void BeginChunk();
		
		// Step 3: Add data to chunks
		void AddTriangle(uint32_t vertexStream, const glm::vec2& v0, const glm::vec2& v1, const glm::vec2& v2);
		void AddTriangle(uint32_t vertexStream, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);
		void AddTriangle(uint32_t vertexStream, const glm::vec4& v0, const glm::vec4& v1, const glm::vec4& v2);
		
		void EndChunk();

		// Step 4: Mesh creation
		bool CreateMesh(Mesh& target);

	private:
		struct StreamDesc
		{
			int32_t m_componentCount;
			std::vector<float> m_streamData;
		};
		struct ChunkStreamData
		{
			uint32_t m_firstEntry;
			uint32_t m_lastEntry;
		};
		struct ChunkDesc
		{
			std::vector<ChunkStreamData> m_streamData;	// 1 per stream
		};

		ChunkDesc m_currentChunk;
		std::vector<StreamDesc> m_streams;
		std::vector<ChunkDesc> m_chunks;
	};
}