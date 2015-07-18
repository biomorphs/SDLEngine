/*
SDLEngine
Matt Hoyle
*/
#include "dds_loader.h"
#include "kernel/assert.h"
#include "kernel/log.h"
#include "kernel/file_io.h"

namespace Render
{
	// DDS Header, from msdn
	struct DDS_HEADER {
		uint32_t m_dwSize;
		uint32_t m_dwFlags;
		uint32_t m_dwHeight;
		uint32_t m_dwWidth;
		uint32_t m_dwPitchOrLinearSize;
		uint32_t m_dwDepth;
		uint32_t m_dwMipMapCount;
		uint32_t m_dwReserved1[11];
		struct DDS_PIXELFORMAT {
			uint32_t m_dwSize;
			uint32_t m_dwFlags;
			uint32_t m_dwFourCC;
			uint32_t m_dwRGBBitCount;
			uint32_t m_dwRBitMask;
			uint32_t m_dwGBitMask;
			uint32_t m_dwBBitMask;
			uint32_t m_dwABitMask;
		} m_pixelFormat;
		uint32_t m_dwCaps;
		uint32_t m_dwCaps2;
		uint32_t m_dwCaps3;
		uint32_t m_dwCaps4;
		uint32_t m_dwReserved2;
	};

	// DDS format fourcc
	constexpr uint32_t MakeFourCC(const char buffer[5])
	{
		return	((uint32_t)buffer[0] << 0) | 
				((uint32_t)buffer[1] << 8) | 
				((uint32_t)buffer[2] << 16) |
				((uint32_t)buffer[3] << 24);
	}
	constexpr uint32_t c_dxt1FourCC = MakeFourCC("DXT1");
	constexpr uint32_t c_dxt3FourCC = MakeFourCC("DXT3");
	constexpr uint32_t c_dxt5FourCC = MakeFourCC("DXT5");

	DDSLoader::DDSLoader()
	{

	}

	DDSLoader::~DDSLoader()
	{

	}

	bool DDSLoader::LoadFile(const char* path, TextureSource& result)
	{
		m_rawBuffer.clear();
		if (!Kernel::FileIO::LoadBinaryFile(path, m_rawBuffer))
		{
			SDE_LOGC(Render, "Failed to load DDS '%s'", path);
			return false;
		}

		if (!IsDDSFormat())
		{
			SDE_LOGC(Render, "File is not dds format", path);
			return false;
		}

		if (!ExtractHeaderData())
		{
			SDE_LOGC(Render, "Failed to extract DDS header");
			return false;
		}

		result = TextureSource(m_width, m_height, m_format, std::move(m_mipDescriptors), std::move(m_rawBuffer));

		return true;
	}

	bool DDSLoader::ExtractHeaderData()
	{
		SDE_ASSERT(m_rawBuffer.size() > (4 + sizeof(DDS_HEADER)));
		const DDS_HEADER* header = reinterpret_cast<const DDS_HEADER*>(m_rawBuffer.data() + 4);
		m_width = header->m_dwWidth;
		m_height = header->m_dwHeight;
		m_format = ExtractFormat(header->m_pixelFormat.m_dwFourCC);
		if (m_format == TextureSource::Format::Unsupported)
		{
			SDE_LOGC(Render, "Unsupported DXT format");
			return false;
		}

		if (!ExtractMips(header->m_dwMipMapCount))
		{
			SDE_LOGC(Render, "Failed to extract mip data");
			return false;
		}

		return true;
	}

	bool DDSLoader::ExtractMips(uint32_t mipCount)
	{
		uint32_t blockSize = (m_format == TextureSource::Format::DXT1) ? 8 : 16;
		uint32_t offset = 4 + sizeof(DDS_HEADER);
		uint32_t width = m_width;
		uint32_t height = m_height;
		m_mipDescriptors.clear();
		m_mipDescriptors.reserve(mipCount);
		for (uint32_t level = 0; level < mipCount && (width || height); ++level)
		{
			uint32_t size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
			TextureSource::MipDesc thisMip;
			thisMip.m_offset = offset;
			thisMip.m_size = size;
			thisMip.m_width = width;
			thisMip.m_height = height;
			m_mipDescriptors.push_back(thisMip);
			offset += size;
			width /= 2;
			height /= 2;
		}
		SDE_ASSERT(offset <= m_rawBuffer.size());
		return true;
	}

	TextureSource::Format DDSLoader::ExtractFormat(uint32_t formatFourcc) const
	{
		switch (formatFourcc)
		{
		case c_dxt1FourCC:
			return TextureSource::Format::DXT1;
		case c_dxt3FourCC:
			return TextureSource::Format::DXT3;
		case c_dxt5FourCC:
			return TextureSource::Format::DXT5;
		default:
			return TextureSource::Format::Unsupported;
		}
	}

	bool DDSLoader::IsDDSFormat() const
	{
		SDE_ASSERT(m_rawBuffer.size() > 4);
		const uint32_t c_ddsFourCC = 0x20534444;	//	'DDS'
		const uint32_t* fourCC = reinterpret_cast<const uint32_t*>(m_rawBuffer.data());
		return (*fourCC == c_ddsFourCC);
	}
}