/*
SDLEngine
Matt Hoyle
*/
#pragma once
#include "kernel/base_types.h"

namespace Render
{
	class TextureSource;
	class Texture
	{
	public:
		Texture();
		~Texture();

		bool Create(const TextureSource& src);
		void Destroy();

		inline uint32_t GetHandle() const { return m_handle; }

	private:
		uint32_t m_handle;
	};
}