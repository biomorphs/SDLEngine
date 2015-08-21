/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include <vector>

namespace DebugGui
{
	class GraphDataBuffer
	{
	public:
		GraphDataBuffer(uint32_t bufferSize);
		~GraphDataBuffer();

		void PushValue(float v);
		const float* GetValues() const	{ return m_valueBuffer.data(); }
		uint32_t ValueCount() const		{ return m_endIndex; }

	private:
		std::vector<float> m_valueBuffer;
		uint32_t m_endIndex;
	};
}