/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include <stdint.h>

// Miscellaneous math functions used all over
namespace Math
{
	// Floating point math
	float SquareRoot(float x);
	float Sin(float a);
	float Cos(float a);
	float Tan(float a);

	// integer math
	uint32_t Log2(uint32_t v);
}

#include "math.inl"