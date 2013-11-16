/*
SDLEngine
Matt Hoyle
*/
#pragma once

struct SDL_Texture;

namespace Render
{
	class Texture
	{
	public:
		Texture(SDL_Texture* theTexture);
		~Texture();

		SDL_Texture* GetTexture();

	private:
		int m_width;
		int m_height;
		SDL_Texture* m_texture;
	};
}