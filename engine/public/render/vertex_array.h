/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include <vector>
#include <stdint.h>

namespace Render
{
	class RenderBuffer;

	enum class VertexDataType : uint8_t
	{
		Float
	};

	// This represents the vertex format state used to render something
	// The aim of this is to reduce draw-time overhead by caching the attribute state of the VBs
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void AddBuffer(uint8_t attribIndex, const RenderBuffer* srcBuffer, VertexDataType srcType, uint8_t components, uint32_t offset = 0, uint32_t stride = 0);
		bool Create();		// Initialises the GL-side state for rendering. Call after buffers have been added!
		void Destroy();

		inline uint32_t GetHandle() const { return m_handle; }

		inline uint32_t GetStreamCount() const { return (uint32_t)m_descriptors.size(); }
		inline VertexDataType GetStreamDataType(uint32_t streamIndex) const	{ return m_descriptors[streamIndex].m_dataType; }
		inline uint8_t GetStreamComponentCount(uint32_t streamIndex) const	{ return m_descriptors[streamIndex].m_componentCount; }
		inline uint8_t GetStreamAttributeIndex(uint32_t streamIndex) const	{ return m_descriptors[streamIndex].m_attribIndex; }

	private:
		uint32_t TranslateDataType(VertexDataType type) const;

		struct VertexBufferDescriptor
		{
			const RenderBuffer* m_srcBuffer;
			size_t m_offset;
			uint32_t m_stride;
			VertexDataType m_dataType;
			uint8_t m_componentCount;
			uint8_t m_attribIndex;
		};
		std::vector<VertexBufferDescriptor> m_descriptors;
		uint32_t m_handle;	// gl handle
	};
}