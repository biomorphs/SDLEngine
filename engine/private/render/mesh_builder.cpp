/*
SDLEngine
Matt Hoyle
*/

#include "mesh_builder.h"
#include "utils.h"

namespace Render
{
	MeshBuilder::MeshBuilder()
	{
	}

	MeshBuilder::~MeshBuilder()
	{
	}

	void MeshBuilder::AddTriangle(uint32_t vertexStream, const glm::vec2& v0, const glm::vec2& v1, const glm::vec2& v2)
	{
		SDE_ASSERT(vertexStream < m_streams.size());
		SDE_ASSERT(m_currentChunk.m_streamData.size() == m_streams.size());

		// this sucks
		auto& streamData = m_streams[vertexStream].m_streamData;
		streamData.reserve(streamData.size() + 6);
		streamData.push_back(v0.x);
		streamData.push_back(v0.y);
		streamData.push_back(v1.x);
		streamData.push_back(v1.y);
		streamData.push_back(v2.x);
		streamData.push_back(v2.y);
		m_currentChunk.m_streamData[vertexStream].m_lastEntry = streamData.size();
	}

	void MeshBuilder::AddTriangle(uint32_t vertexStream, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2)
	{
		SDE_ASSERT(vertexStream < m_streams.size());
		SDE_ASSERT(m_currentChunk.m_streamData.size() == m_streams.size());

		// this sucks
		auto& streamData = m_streams[vertexStream].m_streamData;
		streamData.reserve(streamData.size() + 9);
		streamData.push_back(v0.x);
		streamData.push_back(v0.y);
		streamData.push_back(v0.z);
		streamData.push_back(v1.x);
		streamData.push_back(v1.y);
		streamData.push_back(v1.z);
		streamData.push_back(v2.x);
		streamData.push_back(v2.y);
		streamData.push_back(v2.z);
		m_currentChunk.m_streamData[vertexStream].m_lastEntry = streamData.size();
	}

	void MeshBuilder::AddTriangle(uint32_t vertexStream, const glm::vec4& v0, const glm::vec4& v1, const glm::vec4& v2)
	{
		SDE_ASSERT(vertexStream < m_streams.size());
		SDE_ASSERT(m_currentChunk.m_streamData.size() == m_streams.size());

		// this sucks
		auto& streamData = m_streams[vertexStream].m_streamData;
		streamData.reserve(streamData.size() + 12);
		streamData.push_back(v0.x);
		streamData.push_back(v0.y);
		streamData.push_back(v0.z);
		streamData.push_back(v0.w);
		streamData.push_back(v1.x);
		streamData.push_back(v1.y);
		streamData.push_back(v1.z);
		streamData.push_back(v1.w);
		streamData.push_back(v2.x);
		streamData.push_back(v2.y);
		streamData.push_back(v2.z);
		streamData.push_back(v2.w);
		m_currentChunk.m_streamData[vertexStream].m_lastEntry = streamData.size();
	}

	uint32_t MeshBuilder::AddVertexStream(int32_t componentCount)
	{
		SDE_ASSERT(componentCount < 4);
		SDE_ASSERT(m_currentChunk.m_streamData.size() == 0);
		SDE_ASSERT(m_chunks.size() == 0);
		
		StreamDesc newStream;
		newStream.m_componentCount = componentCount;
		m_streams.push_back(newStream);

		return m_streams.size() - 1;
	}

	void MeshBuilder::BeginChunk()
	{
		SDE_ASSERT(m_streams.size() > 0);
		SDE_ASSERT(m_currentChunk.m_streamData.empty());

		// Initialise chunk stream descriptors
		m_currentChunk.m_streamData.reserve(m_streams.size());
		for (const auto& it : m_streams)
		{
			ChunkStreamData newStreamData;
			newStreamData.m_firstEntry = it.m_streamData.size();
			newStreamData.m_lastEntry = newStreamData.m_firstEntry;
			m_currentChunk.m_streamData.push_back(newStreamData);
		}
	}

	void MeshBuilder::EndChunk()
	{
		m_chunks.push_back(m_currentChunk);
		m_currentChunk.m_streamData.clear();
	}

	bool MeshBuilder::CreateMesh(Mesh& target)
	{
		auto& streams = target.GetStreams();
		auto& vertexArray = target.GetVertexArray();;

		// Fill buffers
		streams.resize(m_streams.size());
		int32_t streamIndex = 0;
		for (const auto& streamIt : m_streams)
		{
			auto& theBuffer = streams[streamIndex];
			auto streamSize = streamIt.m_streamData.size() * sizeof(float);
			theBuffer.Create(streamSize, RenderBufferType::VertexData);
			theBuffer.SetData(0, streamSize, (void*)streamIt.m_streamData.data());
			++streamIndex;
		}

		// Populate vertex array
		streamIndex = 0;
		for (const auto& streamIt : m_streams)
		{
			vertexArray.AddBuffer(streamIndex, &streams[streamIndex], VertexDataType::Float, streamIt.m_componentCount);
			++streamIndex;
		}
		vertexArray.Create();

		return true;
	}
}