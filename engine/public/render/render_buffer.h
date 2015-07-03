/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include <stdint.h>

namespace Render
{
	enum class RenderBufferType : uint32_t
	{
		VertexData,
		IndexData
	};

	enum class RenderBufferModification : uint32_t
	{
		Static,
		Dynamic
	};

	// This represents a single stream of rendering data
	// It could be indices, vertices, whatever
	class RenderBuffer
	{
	public:
		RenderBuffer();
		~RenderBuffer();

		bool Create(size_t bufferSize, RenderBufferType type, RenderBufferModification modification);
		bool Destroy();
		void SetData(size_t offset, size_t size, void* srcData);

		inline uint32_t GetHandle() const { return m_handle; }
		inline size_t GetSize() const { return m_bufferSize; }

	private:
		uint32_t TranslateBufferType(RenderBufferType type) const;
		uint32_t TranslateModificationType(RenderBufferModification type) const;
		size_t m_bufferSize;	// size in bytes
		uint32_t m_handle;
		RenderBufferType m_type;
	};
}