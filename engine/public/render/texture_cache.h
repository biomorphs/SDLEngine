/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "texture.h"
#include "kernel/string_hashing.h"
#include "kernel/object_bucket.h"
#include <map>

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
	
		// GetTexture will return an existing texture if its already there, otherwise it will be loaded
		TextureHandle GetTexture(const char* path);
		
		// Flush will destroy any unreferenced textures
		void Flush();

	private:
		// Texture bucket container
		static const uint32_t c_MaxTextures = 1024;
		typedef ObjectBucket< RefcountedTexture, c_MaxTextures, HeapBucketAllocator > TextureBucket;

		// TextureMap is used for fast lookups into the bucket
		typedef std::map<StringHash, RefcountedTexture*> TextureMap;
		typedef std::pair<StringHash, RefcountedTexture*> TexturePair;
		TextureMap m_textureMap;
		
		// TextureBucket is used so handle internals won't move around
		TextureBucket m_textureBucket;

		Device* m_device;
	};
}