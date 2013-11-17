/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "kernel/string_hashing.h"
#include <map>

struct SDL_Texture;

namespace Render
{
	class Texture;
	class Device;

	// This represents a central system for loading + storing all textures
	class TextureCache
	{
	public:
		TextureCache(Device* renderDevice);
		~TextureCache();
		
		// Load will return an existing texture if its already there
		Texture* LoadTexture(const char* path);
		void DestroyTexture(Texture* theTexture);

	private:
		typedef std::map<StringHash, Texture*> TextureMap;
		typedef std::pair<StringHash, Texture*> TexturePair;
		TextureMap m_textures;
		Device* m_device;
	};
}