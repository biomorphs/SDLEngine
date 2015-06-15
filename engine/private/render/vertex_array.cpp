/*
SDLEngine
Matt Hoyle
*/
#include "vertex_array.h"
#include "utils.h"
#include "render_buffer.h"

namespace Render
{
	VertexArray::VertexArray()
		: m_handle(0)
	{
	}

	VertexArray::~VertexArray()
	{
		SDE_ASSERT(m_handle == 0, "VertexArray leaked");
	}

	void VertexArray::AddBuffer(uint8_t attribIndex, const RenderBuffer* srcBuffer, VertexDataType srcType, uint8_t components, uint32_t offset, uint32_t stride)
	{
		SDE_ASSERT(srcBuffer != nullptr);
		SDE_ASSERT(components <= 4);

		// blindly add them, no real error checking, since (hopefully) Create will deal with it
		VertexBufferDescriptor newDesc;
		newDesc.m_srcBuffer = srcBuffer;
		newDesc.m_dataType = srcType;
		newDesc.m_componentCount = components;
		newDesc.m_offset = offset;
		newDesc.m_stride = stride;
		newDesc.m_attribIndex = attribIndex;

		m_descriptors.push_back(newDesc);
	}

	inline uint32_t VertexArray::TranslateDataType(VertexDataType type) const
	{
		switch (type)
		{
		case VertexDataType::Float:
			return GL_FLOAT;
		default:
			return -1;
		}
	}

	bool VertexArray::Create()
	{
		SDE_ASSERT(m_handle == 0);

		glGenVertexArrays(1, &m_handle);
		SDE_RENDER_PROCESS_GL_ERRORS_RET("glGenVertexArrays");

		glBindVertexArray(m_handle);
		SDE_RENDER_PROCESS_GL_ERRORS_RET("glBindVertexArray");

		for (auto it = m_descriptors.begin(); it != m_descriptors.end(); ++it)
		{
			// We bind the VBs now, so we don't have to do it for each draw call
			// Whenever we bind this VAO from now on, these buffers will also be bound
			glBindBuffer(GL_ARRAY_BUFFER, it->m_srcBuffer->GetHandle());
			SDE_RENDER_PROCESS_GL_ERRORS_RET("glBindBuffer");

			uint32_t glDataType = TranslateDataType(it->m_dataType);
			SDE_ASSERT(glDataType != -1);

			// cache the attributes in this VAO
			glVertexAttribPointer(it->m_attribIndex, it->m_componentCount, glDataType, false, it->m_stride, (void*)it->m_offset);
			SDE_RENDER_PROCESS_GL_ERRORS_RET("glVertexAttribPointer");

			// Enable the stream
			glEnableVertexAttribArray(it->m_attribIndex);
			SDE_RENDER_PROCESS_GL_ERRORS_RET("glEnableVertexAttribArray");
		}

		// Reset state
		glBindVertexArray(0);
		SDE_RENDER_PROCESS_GL_ERRORS_RET("glBindVertexArray");

		glBindBuffer(GL_ARRAY_BUFFER, 0);	
		SDE_RENDER_PROCESS_GL_ERRORS_RET("glBindBuffer");

		return true;
	}

	void VertexArray::Destroy()
	{
		// Note we do not destroy the buffers, only the VAO
		glDeleteVertexArrays(1, &m_handle);
		SDE_RENDER_PROCESS_GL_ERRORS("glDeleteVertexArrays");

		m_handle = 0;
	}
}