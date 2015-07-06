/*
SDLEngine
Matt Hoyle
*/

namespace Math
{
	namespace BitTwiddling
	{
		inline uint32_t Log2(uint32_t v)
		{
			// This should be supported on all modern-ish CPUs
			unsigned long result = 0;
			return _BitScanReverse(&result, v) != 0 ? result : 0;
		}
	}
}