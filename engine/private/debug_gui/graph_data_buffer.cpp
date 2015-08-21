/*
SDLEngine
Matt Hoyle
*/

#include "graph_data_buffer.h"
#include <algorithm>

namespace DebugGui
{
	GraphDataBuffer::GraphDataBuffer(uint32_t bufferSize)
		: m_endIndex(0)
	{
		m_valueBuffer.resize(bufferSize);
	}

	GraphDataBuffer::~GraphDataBuffer()
	{

	}

	void GraphDataBuffer::PushValue(float v)
	{
		if (m_endIndex == m_valueBuffer.size())
		{
			m_valueBuffer.erase(m_valueBuffer.begin());
			m_valueBuffer.push_back(v);
		}
		else
		{
			m_valueBuffer[m_endIndex++] = v;
		}
	}
}