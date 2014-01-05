/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "sprite_definition.h"
#include "math/rect.h"

namespace Render
{
	// Represents a single sprite to be drawn
	class SpriteInstance
	{
	public:
		SpriteDefHandle m_spriteDefinition;
		Math::Rect m_destination;
		double m_rotation;			// degrees
		Math::Vector2 m_rotOrigin;	// point to rotate around
	};
}