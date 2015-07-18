/*
SDLEngine
Matt Hoyle
*/
#include "texture.h"
#include "texture_source.h"
#include "utils.h"

namespace Render
{
	Texture::Texture()
		: m_handle(0)
	{
	}

	Texture::~Texture()
	{
		Destroy();
	}

	uint32_t SourceFormatToGL(TextureSource::Format f)
	{
		switch (f)
		{
		case TextureSource::Format::DXT1:
			return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		case TextureSource::Format::DXT3:
			return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		case TextureSource::Format::DXT5:
			return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		default:
			return -1;
		}
	}

	bool Texture::Create(const TextureSource& src)
	{
		SDE_RENDER_ASSERT(src.MipCount() > 0);
		SDE_RENDER_ASSERT(m_handle == 0);

		glGenTextures(1, &m_handle);
		SDE_RENDER_PROCESS_GL_ERRORS_RET("glGenTextures");

		glBindTexture(GL_TEXTURE_2D, m_handle);
		SDE_RENDER_PROCESS_GL_ERRORS_RET("glBindTexture");

		uint32_t glFormat = SourceFormatToGL(src.SourceFormat());
		SDE_RENDER_ASSERT(glFormat != -1);
	
		const uint32_t mipCount = src.MipCount();
		for (uint32_t m = 0; m < mipCount; ++m)
		{
			uint32_t w = 0, h = 0;
			size_t size = 0;
			const uint8_t* mipData = src.MipLevel(m, w, h, size);
			SDE_ASSERT(mipData);

			glCompressedTexImage2D(GL_TEXTURE_2D, m, glFormat, w, h, 0, (GLsizei)size, mipData);
			SDE_RENDER_PROCESS_GL_ERRORS_RET("glCompressedTexImage2D");
		}

		// This should probably be tied to sampler state, but for now we will just use bilinear for everything
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
		SDE_RENDER_PROCESS_GL_ERRORS_RET("glBindTexture");

		return true;
	}

	void Texture::Destroy()
	{
		if (m_handle != 0)
		{
			glDeleteTextures(1, &m_handle);
			SDE_RENDER_PROCESS_GL_ERRORS("glDeleteTextures");
			m_handle = 0;
		}
	}
}