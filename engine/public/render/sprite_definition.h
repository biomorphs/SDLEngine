/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "texture.h"
#include "math/rect.h"
#include "kernel/refcounted_data.h"
#include "kernel/handle.h"

// This represents the prototype for a sprite
namespace Render
{
	class SpriteDefinition
	{
	public:
		SpriteDefinition() { }
		SpriteDefinition(const SpriteDefinition& other) { m_sourceRect = other.m_sourceRect; m_texture = other.m_texture; }
		~SpriteDefinition() { }
		Math::Rect m_sourceRect;		// Source offset into the texture (pixels)
		TextureHandle m_texture;		// Handle to the source texture
	};

	// SpriteDefinition handle types
	typedef RefcountedData<SpriteDefinition> RefcountedSpriteDefinition;
	typedef Handle< RefcountedSpriteDefinition > SpriteDefHandle;
}