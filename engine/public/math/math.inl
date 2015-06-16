#include <math.h>
#include <intrin.h>

namespace Math
{

	inline float SquareRoot(float x)
	{
		return sqrtf(x);
	}

	inline float Sin(float a)
	{
		return sinf(a);
	}

	inline float Cos(float a)
	{
		return cosf(a);
	}

	inline float Tan(float a)
	{
		return tanf(a);
	}

	uint32_t Log2(uint32_t v)
	{
		// This should be supported on all modern-ish CPUs
		unsigned long result = 0;
		return _BitScanReverse(&result, v) != 0 ? result : 0;
	}
}