/*
SDLEngine
Matt Hoyle
*/

#include "texture_cache.h"
#include "texture.h"
#include "device.h"
#include "utils.h"
#include "kernel/assert.h"
#include "SDL_Image.h"

namespace Render
{
	TextureCache::TextureCache(Device* renderDevice)
		: m_device(renderDevice)
	{
		SDE_ASSERT(m_device);
	}
	
	TextureCache::~TextureCache()
	{
		SDE_ASSERT(m_textures.size() == 0, "There are still textures loaded!");
	}

	Texture* TextureCache::LoadTexture(const char* path)
	{
		StringHash pathHash = StringHashing::GetHash(path);
		auto it = m_textures.find(pathHash);
		if (it != m_textures.end())
		{
			return (*it).second;
		}

		SDL_Texture* loadedTexture = IMG_LoadTexture(m_device->GetRenderer(), path);
		SDE_RENDER_ASSERT(loadedTexture);

		if (loadedTexture)
		{
			Texture* texture = new Texture(loadedTexture);
			m_textures.insert(TexturePair(pathHash, texture));
			return texture;
		}

		return nullptr;
	}

	void TextureCache::DestroyTexture(Texture* theTexture)
	{
		auto it = m_textures.begin();
		for (; it != m_textures.end(); ++it)
		{
			if (it->second == theTexture)
				break;
		}

		if (it != m_textures.end())
		{
			delete it->second;
			m_textures.erase(it);
		}
	}
}