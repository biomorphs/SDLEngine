/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include <stdint.h>

namespace Render
{
	// This represents a single stream of rendering data
	// It could be indices, vertices, whatever
	class RenderBuffer
	{
	public:
		RenderBuffer();
		~RenderBuffer();

		bool Create(size_t bufferSize);
		bool Destroy();
		void SetData(size_t offset, size_t size, void* srcData);

		inline uint32_t GetHandle() const { return m_handle; }
		inline size_t GetSize() const { return m_bufferSize; }

	private:
		size_t m_bufferSize;	// size in bytes
		uint32_t m_handle;
	};
}