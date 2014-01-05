/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "math.h"

// Simple floating point 2d vector
namespace Math
{
	class Vector2
	{
	public:
		Vector2();
		Vector2(int32_t x, int32_t y);
		Vector2(int32_t v);

		bool operator!=(const Vector2& t) const;
		bool operator==(const Vector2& t) const;

		Vector2 operator*(float f) const;
		void operator*=(float f);
		Vector2 operator+(Vector2 f) const;
		void operator+=(Vector2 f);
		Vector2 operator-(Vector2 f) const;
		void operator-=(Vector2 f);

		float Length() const;
		Vector2 Normalised() const;

		int32_t m_x;
		int32_t m_y;
	};
}

#include "vector2.inl"