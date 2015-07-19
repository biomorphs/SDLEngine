/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "kernel/base_types.h"
#include <vector>

namespace Render
{
	class TextureSource
	{
	public:
		enum class Format
		{
			DXT1,
			DXT3,
			DXT5,
			AlphaOneByte,
			Unsupported
		};
		struct MipDesc
		{
			uint32_t m_width;
			uint32_t m_height;
			uintptr_t m_offset;
			size_t m_size;
		};

		TextureSource();
		TextureSource(uint32_t w, uint32_t h, Format f, std::vector<MipDesc>&& mips, std::vector<uint8_t>&& data);
		~TextureSource();

		inline uint32_t Width() const { return m_width; }
		inline uint32_t Height() const { return m_height; }
		inline uint32_t MipCount() const { return static_cast<uint32_t>(m_mipDescriptors.size()); }
		inline Format SourceFormat() const { return m_format; }
		const uint8_t* MipLevel(uint32_t mip, uint32_t& w, uint32_t& h, size_t& size) const;

	private:
		Format m_format;
		uint32_t m_width;
		uint32_t m_height;
		std::vector<MipDesc> m_mipDescriptors;
		std::vector<uint8_t> m_rawBuffer;
	};
}

#include "texture_source.inl"