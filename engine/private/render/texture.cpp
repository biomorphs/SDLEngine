/*
SDLEngine
Matt Hoyle
*/
#include "texture.h"
#include "kernel/assert.h"
#include "SDL.h"

namespace Render
{
	Texture::Texture(SDL_Texture* theTexture)
		: m_texture(theTexture)
	{
		SDE_ASSERT(theTexture);
		SDL_QueryTexture(theTexture, nullptr, nullptr, &m_width, &m_height);
	}

	Texture::~Texture()
	{
		SDE_ASSERT(m_texture);
		SDL_DestroyTexture(m_texture);
		m_texture = nullptr;
	}

	SDL_Texture* Texture::GetTexture()
	{
		return m_texture;
	}
}