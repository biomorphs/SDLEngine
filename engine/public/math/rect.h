/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "vector2.h"

// A simple rectangle container with transformation 
namespace Math
{
	class Rect
	{
	public:
		Rect();
		Rect(Vector2 origin, Vector2 size);
		Rect(int32_t x, int32_t y, int32_t w, int32_t h);

		bool operator!=(const Rect& t) const;
		bool operator==(const Rect& t) const;

		Rect Translate(Vector2 translation);
		Rect Scale(float scaleX, float scaleY);

		Vector2 m_origin;
		Vector2 m_size;
	};
}

#include "rect.inl"