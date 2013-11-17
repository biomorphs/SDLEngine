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
		SDE_ASSERT(m_textureMap.size() == 0, "There are still textures loaded!");
	}

	TextureCache::TextureHandle TextureCache::GetTexture(const char* path)
	{
		// If it already exists, return a handle to the existing one
		StringHash pathHash = StringHashing::GetHash(path);
		auto it = m_textureMap.find(pathHash);
		if (it != m_textureMap.end())
		{
			return TextureHandle((*it).second);
		}

		SDL_Texture* loadedTexture = IMG_LoadTexture(m_device->GetRenderer(), path);
		SDE_RENDER_ASSERT(loadedTexture, "Failed to load texture '%s'", path);
		if (loadedTexture)
		{
			// Get a new data handle
			TextureData* textureWrapper = m_textureBucket.AddObject();
			SDE_ASSERT(textureWrapper);

			// Create the texture and pass it to the data handle
			Texture* texture = new Texture(loadedTexture);
			SDE_ASSERT(texture);
			*textureWrapper = std::move( TextureData(texture) );

			// Add it to the name lookup
			m_textureMap.insert(TexturePair(pathHash, textureWrapper));

			return TextureHandle( textureWrapper );
		}

		return TextureHandle();		// Empty handle on error
	}

	void TextureCache::Flush()
	{
		for (auto it = m_textureBucket.Begin(); it != m_textureBucket.End(); ++it)
		{
			if ((*it).GetReferenceCount() == 0)
			{
				// First, remove it from the name lookup
				auto mapIt = m_textureMap.begin();
				for (; mapIt != m_textureMap.end(); ++mapIt)
				{
					if (mapIt->second == &(*it))
						break;
				}
				m_textureMap.erase(mapIt);

				// Delete the texture wrapper
				delete (*it).GetData();

				// Free the wrapped object
				m_textureBucket.RemoveObject(&(*it));

				it = m_textureBucket.Begin();
			}
		}
	}
}