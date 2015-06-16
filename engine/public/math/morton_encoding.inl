/*
SDLEngine
Matt Hoyle
*/
#pragma once
#include "morton_encoding.h"

namespace Math
{
	// Helper function definitions
	namespace _Internal
	{
		inline uint64_t SplitIntegerInto3(uint32_t a);
		inline uint32_t CompactMortonIntoOriginal(uint64_t a);
	}	

	// Encode returns a 64 bit key for 3 axes
	inline uint64_t MortonEncode(uint32_t x, uint32_t y, uint32_t z)
	{
		uint64_t key = 0ull;
		key = _Internal::SplitIntegerInto3(x);
		key |= _Internal::SplitIntegerInto3(y) << 1;
		key |= _Internal::SplitIntegerInto3(z) << 2;
		return key;
	}

	

	// Decode returns the 3 split coordinates for a key
	inline void MortonDecode(uint64_t key, uint32_t &x, uint32_t& y, uint32_t& z)
	{
		x = _Internal::CompactMortonIntoOriginal(key);
		y = _Internal::CompactMortonIntoOriginal(key >> 1);
		z = _Internal::CompactMortonIntoOriginal(key >> 2);
	}

	namespace _Internal
	{
		// method to seperate bits from a given integer 3 positions apart
		// shamelessly taken from http://www.forceflow.be/2013/10/07/morton-encodingdecoding-through-bit-interleaving-implementations/
		inline uint64_t SplitIntegerInto3(uint32_t a)
		{
			uint64_t x = a & 0x1fffff;				// we only look at the first 21 bits
			x = (x | x << 32) & 0x1f00000000ffff;	// shift left 32 bits, OR with self, and 00011111000000000000000000000000000000001111111111111111
			x = (x | x << 16) & 0x1f0000ff0000ff;	// shift left 32 bits, OR with self, and 00011111000000000000000011111111000000000000000011111111
			x = (x | x << 8) & 0x100f00f00f00f00f;	// shift left 32 bits, OR with self, and 0001000000001111000000001111000000001111000000001111000000000000
			x = (x | x << 4) & 0x10c30c30c30c30c3;	// shift left 32 bits, OR with self, and 0001000011000011000011000011000011000011000011000011000100000000
			x = (x | x << 2) & 0x1249249249249249;
			return x;
		}

		// Method to compact a single morton encoded axis back into its original coordinate
		// Masks and shift orders are simply reversed
		inline uint32_t CompactMortonIntoOriginal(uint64_t a)
		{
			a &= 0x1249249249249249;
			a = (a | a >> 2) & 0x10c30c30c30c30c3;
			a = (a | a >> 4) & 0x100f00f00f00f00f;
			a = (a | a >> 8) & 0x1f0000ff0000ff;
			a = (a | a >> 16) & 0x1f00000000ffff;
			a = (a | a >> 32) & 0x1fffff;
			return static_cast<uint32_t>( a );
		}
	}	
}