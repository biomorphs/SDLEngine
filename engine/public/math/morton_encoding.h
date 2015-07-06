/*
SDLEngine
Matt Hoyle
*/
#pragma once

#include "kernel/base_types.h"

namespace Math
{
	// Morton encoding is a way of splitting a coordinate system into a z-order curve
	// This gives us spatial locality for nearest neighbours
	// It works by splitting each axis coordinate into multiple shifted bits
	// that can then be used to lookup into a linear dataset
	// This implementation uses a 64 bit key, which can represent at max 3 axes of 21 bits each

	// Encode returns a 64 bit key for 3 axes
	uint64_t MortonEncode(uint32_t x, uint32_t y, uint32_t z);

	// Decode returns the 3 split coordinates for a key
	void MortonDecode(uint64_t key, uint32_t &x, uint32_t& y, uint32_t& z);
}

#include "morton_encoding.inl"