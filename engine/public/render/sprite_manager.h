/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "kernel/refcounted_data.h"
#include "kernel/handle.h"
#include "sprite_definition.h"
#include "kernel/object_bucket.h"
#include <map>

namespace Render
{
	// This class handles a set of sprite definitions
	class SpriteManager
	{
	public:
		SpriteManager();
		~SpriteManager();

		SpriteDefHandle AddSpriteDefinition(const TextureHandle& texture, const Math::Rect& spriteRect);
		void Flush();

	private:
		static const uint32_t c_MaxSpriteDefinitions = 1024 * 8;
		typedef ObjectBucket< RefcountedSpriteDefinition, c_MaxSpriteDefinitions, HeapBucketAllocator > SpriteBucket;

		SpriteBucket m_spriteBucket;
	};
}