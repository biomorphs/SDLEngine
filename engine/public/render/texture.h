/*
SDLEngine
Matt Hoyle
*/
#pragma once

struct SDL_Texture;

#include "kernel/refcounted_data.h"
#include "kernel/handle.h"

namespace Render
{
	class Texture
	{
	public:
		Texture();
		Texture(SDL_Texture* theTexture);
		~Texture();

		void Release();
		SDL_Texture* GetTexture();

	private:
		int m_width;
		int m_height;
		SDL_Texture* m_texture;
	};

	// Texture handle types
	typedef RefcountedData<Texture> RefcountedTexture;
	typedef Handle< RefcountedTexture > TextureHandle;
}