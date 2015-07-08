/*
SDLEngine
Matt Hoyle
*/

#include "mesh_builder.h"
#include "utils.h"
#include <gtc/type_ptr.hpp>

namespace Render
{
	MeshBuilder::MeshBuilder()
		: m_currentVertexIndex(0)
	{
	}

	MeshBuilder::~MeshBuilder()
	{
	}

	bool MeshBuilder::HasData()
	{
		if (m_chunks.size() > 0)
		{
			return (m_chunks[0].m_lastVertex != 0);
		}
		else
		{
			return 0;
		}
	}

	void MeshBuilder::SetStreamData(uint32_t vertexStream, float v0, float v1, float v2)
	{
		SDE_ASSERT(vertexStream < m_streams.size());
		SDE_ASSERT(m_streams[vertexStream].m_componentCount == 1);
		auto& streamData = m_streams[vertexStream].m_streamData;
		streamData.push_back(v0);
		streamData.push_back(v1);
		streamData.push_back(v2);
	}

	void MeshBuilder::SetStreamData(uint32_t vertexStream, const glm::vec2& v0, const glm::vec2& v1, const glm::vec2& v2)
	{
		SDE_ASSERT(vertexStream < m_streams.size());
		SDE_ASSERT(m_streams[vertexStream].m_componentCount == 2);

		auto& streamData = m_streams[vertexStream].m_streamData;
		streamData.insert(streamData.end(), glm::value_ptr(v0), glm::value_ptr(v0) + 2);
		streamData.insert(streamData.end(), glm::value_ptr(v1), glm::value_ptr(v1) + 2);
		streamData.insert(streamData.end(), glm::value_ptr(v2), glm::value_ptr(v2) + 2);
	}

	void MeshBuilder::SetStreamData(uint32_t vertexStream, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2)
	{
		SDE_ASSERT(vertexStream < m_streams.size());
		SDE_ASSERT(m_streams[vertexStream].m_componentCount == 3);

		auto& streamData = m_streams[vertexStream].m_streamData;
		streamData.insert(streamData.end(), glm::value_ptr(v0), glm::value_ptr(v0) + 3);
		streamData.insert(streamData.end(), glm::value_ptr(v1), glm::value_ptr(v1) + 3);
		streamData.insert(streamData.end(), glm::value_ptr(v2), glm::value_ptr(v2) + 3);
	}

	void MeshBuilder::SetStreamData(uint32_t vertexStream, const glm::vec4& v0, const glm::vec4& v1, const glm::vec4& v2)
	{
		SDE_ASSERT(vertexStream < m_streams.size());
		SDE_ASSERT(m_streams[vertexStream].m_componentCount == 4);

		auto& streamData = m_streams[vertexStream].m_streamData;
		streamData.insert(streamData.end(), glm::value_ptr(v0), glm::value_ptr(v0) + 4);
		streamData.insert(streamData.end(), glm::value_ptr(v1), glm::value_ptr(v1) + 4);
		streamData.insert(streamData.end(), glm::value_ptr(v2), glm::value_ptr(v2) + 4);
	}

	uint32_t MeshBuilder::AddVertexStream(int32_t componentCount)
	{
		SDE_ASSERT(componentCount <= 4);
		SDE_ASSERT(m_chunks.size() == 0);
		
		StreamDesc newStream;
		newStream.m_componentCount = componentCount;
		m_streams.push_back(newStream);

		return static_cast<uint32_t>( m_streams.size() - 1 );
	}

	void MeshBuilder::BeginTriangle()
	{
	}

	void MeshBuilder::EndTriangle()
	{
		m_currentVertexIndex += 3;

		// Make sure all streams have data
#ifdef SDE_DEBUG
		for (auto stream : m_streams)
		{
			SDE_ASSERT((stream.m_streamData.size() / stream.m_componentCount) == m_currentVertexIndex);
		}
#endif
	}

	void MeshBuilder::BeginChunk()
	{
		SDE_ASSERT(m_streams.size() > 0);

		m_currentChunk.m_firstVertex = m_currentVertexIndex;
		m_currentChunk.m_lastVertex = m_currentVertexIndex;
	}

	void MeshBuilder::EndChunk()
	{
		m_currentChunk.m_lastVertex = m_currentVertexIndex;
		if ((m_currentChunk.m_lastVertex - m_currentChunk.m_firstVertex) > 0)
		{
			m_chunks.push_back(m_currentChunk);
		}		
	}

	bool MeshBuilder::ShouldRecreateMesh(Mesh& target)
	{
		const size_t c_maximumWaste = 8 * 1024;
		// If the mesh streams match, and the stream buffers are big enough, then we don't need to do anything
		auto& streams = target.GetStreams();
		auto& vertexArray = target.GetVertexArray();

		if (streams.size() != m_streams.size())
		{
			return true;
		}
		if (vertexArray.GetStreamCount() != m_streams.size())
		{
			return true;
		}

		for (int32_t streamIndex = 0; streamIndex < streams.size(); ++streamIndex)
		{
			const auto thisStreamSize = m_streams[streamIndex].m_streamData.size() * sizeof(float);
			if (streams[streamIndex].GetSize() < thisStreamSize)
			{
				return true;
			}
			if (streams[streamIndex].GetSize() > (thisStreamSize + c_maximumWaste))
			{
				return true;
			}
			if (vertexArray.GetStreamAttributeIndex(streamIndex) != streamIndex)
			{
				return true;
			}
			if (vertexArray.GetStreamComponentCount(streamIndex) != m_streams[streamIndex].m_componentCount)
			{
				return true;
			}
			if (vertexArray.GetStreamDataType(streamIndex) != Render::VertexDataType::Float)
			{
				return true;
			}
		}

		return false;
	}

	void MeshBuilder::RecreateMesh(Mesh& target)
	{
		auto& streams = target.GetStreams();
		auto& vertexArray = target.GetVertexArray();

		for (auto& s : streams)
		{
			s.Destroy();
		}
		streams.clear();
		streams.resize(m_streams.size());
		int32_t streamIndex = 0;
		for (const auto& streamIt : m_streams)
		{
			auto& theBuffer = streams[streamIndex];
			auto streamSize = streamIt.m_streamData.size() * sizeof(float);
			theBuffer.Create(streamSize, RenderBufferType::VertexData, RenderBufferModification::Static);
			++streamIndex;
		}

		streamIndex = 0;
		vertexArray.Destroy();
		for (const auto& streamIt : m_streams)
		{
			vertexArray.AddBuffer(streamIndex, &streams[streamIndex], VertexDataType::Float, streamIt.m_componentCount);
			++streamIndex;
		}
		vertexArray.Create();
	}

	bool MeshBuilder::CreateMesh(Mesh& target)
	{
		auto& streams = target.GetStreams();
		auto& vertexArray = target.GetVertexArray();
		auto& chunks = target.GetChunks();

		if (ShouldRecreateMesh(target))
		{
			RecreateMesh(target);
		}

		// Fill buffers
		int32_t streamIndex = 0;
		for (const auto& streamIt : m_streams)
		{
			auto& theBuffer = streams[streamIndex];
			auto streamSize = streamIt.m_streamData.size() * sizeof(float);
			theBuffer.SetData(0, streamSize, (void*)streamIt.m_streamData.data());
			++streamIndex;
		}

		// Populate chunks. We always rebuild this data
		chunks.clear();
		chunks.reserve(m_chunks.size());
		for (const auto& chunk : m_chunks)
		{
			chunks.emplace_back(chunk.m_firstVertex, chunk.m_lastVertex - chunk.m_firstVertex, Render::PrimitiveType::Triangles);
		}

		return true;
	}
}