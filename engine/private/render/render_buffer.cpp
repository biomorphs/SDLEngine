/*
SDLEngine
Matt Hoyle
*/
#include "render_buffer.h"
#include "utils.h"
#include <glew.h>

namespace Render
{
	RenderBuffer::RenderBuffer()
		: m_bufferSize(0)
		, m_handle(0)
	{
	}

	RenderBuffer::~RenderBuffer()
	{
		if (m_bufferSize > 0 && m_handle != 0)
		{
			SDE_RENDER_ASSERT(false, "RenderBuffer leak. Destroy() must be called");
		}
	}

	bool RenderBuffer::Create(size_t bufferSize)
	{
		SDE_RENDER_ASSERT(bufferSize > 0, "Buffer size must be >0");

		if (bufferSize > 0)
		{
			glGenBuffers(1, &m_handle);
			SDE_RENDER_PROCESS_GL_ERRORS_RET("glGenBuffers");

			glBindBuffer(GL_ARRAY_BUFFER, m_handle);
			SDE_RENDER_PROCESS_GL_ERRORS_RET("glBindBuffer");

			// We initialise the buffer memory to the correct size, but we do NOT populate it
			glBufferData(GL_ARRAY_BUFFER, bufferSize, nullptr, GL_STATIC_DRAW);
			SDE_RENDER_PROCESS_GL_ERRORS_RET("glBufferData");

			m_bufferSize = bufferSize;

			// Reset state
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			SDE_RENDER_PROCESS_GL_ERRORS_RET("glBindBuffer");
		}

		return true;
	}

	void RenderBuffer::SetData(size_t offset, size_t size, void* srcData)
	{
		SDE_ASSERT(offset < m_bufferSize);
		SDE_ASSERT((size + offset) <= m_bufferSize);
		SDE_ASSERT(srcData != nullptr);
		SDE_ASSERT(m_handle != 0);

		glBindBuffer(GL_ARRAY_BUFFER, m_handle);
		SDE_RENDER_PROCESS_GL_ERRORS("glBindBuffer");

		glBufferSubData(GL_ARRAY_BUFFER, offset, size, srcData);
		SDE_RENDER_PROCESS_GL_ERRORS("glBufferSubData");

		glBindBuffer(GL_ARRAY_BUFFER, 0);	// Reset state
		SDE_RENDER_PROCESS_GL_ERRORS("glBindBuffer");
	}

	bool RenderBuffer::Destroy()
	{
		if (m_bufferSize > 0)
		{
			glDeleteBuffers(1, &m_handle);
			SDE_RENDER_PROCESS_GL_ERRORS("glDeleteBuffers");
			m_handle = 0;
		}

		return true;
	}
}