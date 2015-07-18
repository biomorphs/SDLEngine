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

	bool Texture::CreateSimpleTexture(const TextureSource& src)
	{
		glGenTextures(1, &m_handle);
		SDE_RENDER_PROCESS_GL_ERRORS_RET("glGenTextures");

		glBindTexture(GL_TEXTURE_2D, m_handle);
		SDE_RENDER_PROCESS_GL_ERRORS_RET("glBindTexture");

		uint32_t glFormat = SourceFormatToGL(src.SourceFormat());
		SDE_RENDER_ASSERT(glFormat != -1);

		const uint32_t mipCount = src.MipCount();

		// This preallocates the entire mip-chain
		glTexStorage2D(GL_TEXTURE_2D, mipCount, glFormat, src.Width(), src.Height());
		SDE_RENDER_PROCESS_GL_ERRORS_RET("glTexStorage2D");

		for (uint32_t m = 0; m < mipCount; ++m)
		{
			uint32_t w = 0, h = 0;
			size_t size = 0;
			const uint8_t* mipData = src.MipLevel(m, w, h, size);
			SDE_ASSERT(mipData);

			glCompressedTexSubImage2D(GL_TEXTURE_2D, m, 0, 0, w, h, glFormat, (GLsizei)size, mipData);
			SDE_RENDER_PROCESS_GL_ERRORS_RET("glCompressedTexSubImage2D");
		}

		// This should probably be tied to sampler state, but for now we will just use bilinear for everything
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
		SDE_RENDER_PROCESS_GL_ERRORS_RET("glBindTexture");

		return true;
	}

	bool Texture::CreateArrayTexture(const std::vector<TextureSource>& src)
	{
		glGenTextures(1, &m_handle);
		SDE_RENDER_PROCESS_GL_ERRORS_RET("glGenTextures");

		glBindTexture(GL_TEXTURE_2D_ARRAY, m_handle);
		SDE_RENDER_PROCESS_GL_ERRORS_RET("glBindTexture");

		uint32_t glFormat = SourceFormatToGL(src[0].SourceFormat());
		SDE_RENDER_ASSERT(glFormat != -1);

		const uint32_t mipCount = src[0].MipCount();

		// This preallocates the entire mip-chain for all textures in the array
		glTexStorage3D(GL_TEXTURE_2D_ARRAY, mipCount, glFormat, src[0].Width(), src[0].Height(), (GLsizei)src.size());
		SDE_RENDER_PROCESS_GL_ERRORS_RET("glTexStorage3D");

		// Now, since opengl is retarded and requires that glCompressedTexSubImage3D is passed all data for a mip level,
		// we must manually pack all data for a particular mip into one buffer
		uint32_t mip0w, mip0h;
		size_t mip0size;
		src[0].MipLevel(0, mip0w, mip0h, mip0size);	// Use first image to get total size required
		std::vector<uint8_t> mipBuffer(mip0size * src.size());
		for (uint32_t m = 0; m < mipCount; ++m)
		{
			uint32_t w = 0, h = 0;
			size_t size = 0;
			uint8_t* dataPtr = mipBuffer.data();
			for (uint32_t d = 0; d < src.size(); ++d)
			{
				const uint8_t* mipData = src[d].MipLevel(m, w, h, size);
				SDE_ASSERT(mipData);
				memcpy(dataPtr, mipData, size);
				dataPtr += size;
			}
			auto bytesWritten = (size_t)(dataPtr - mipBuffer.data());
			SDE_ASSERT(bytesWritten <= mipBuffer.size());

			glCompressedTexSubImage3D(GL_TEXTURE_2D_ARRAY, m, 0, 0, 0, w, h, (GLsizei)src.size(), glFormat, (GLsizei)(bytesWritten), mipBuffer.data());
			SDE_RENDER_PROCESS_GL_ERRORS_RET("glCompressedTexSubImage3D");
		}
		mipBuffer.clear();

		// This should probably be tied to sampler state, but for now we will just use bilinear for everything
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
		SDE_RENDER_PROCESS_GL_ERRORS_RET("glBindTexture");

		return true;
	}

	bool Texture::ValidateSource(const std::vector<TextureSource>& src)
	{
		if (src.size() == 0)
		{
			return false;
		}
		if (src[0].MipCount() == 0)
		{
			return false;
		}

		uint32_t mipCount = src[0].MipCount();
		uint32_t width = src[0].Width();
		uint32_t height = src[0].Height();
		TextureSource::Format format = src[0].SourceFormat();
		for (auto& it : src)
		{
			if (mipCount != it.MipCount())
			{
				return false;
			}
			if (width != it.Width())
			{
				return false;
			}
			if (height != it.Height())
			{
				return false;
			}
			if (format != it.SourceFormat())
			{
				return false;
			}
		}

		return true;
	}

	bool Texture::Create(const std::vector<TextureSource>& src)
	{
		SDE_RENDER_ASSERT(m_handle == 0);
		SDE_ASSERT(src.size() > 0);
		if (!ValidateSource(src))
		{
			SDE_LOGC("Source data not valid for this texture");
			return false;
		}

		bool createOk = false;
		if (src.size() == 1)
		{
			m_isArray = false;
			return CreateSimpleTexture(src[0]);
		}
		else
		{
			m_isArray = true;
			return CreateArrayTexture(src);
		}

		return createOk;
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