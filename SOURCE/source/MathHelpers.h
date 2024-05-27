#pragma once
#include <cmath>
#include <float.h>

namespace dae
{
	/* --- CONSTANTS --- */
	constexpr float PI{ 3.14159265358979323846f };
	constexpr float PI_DIV_2{ 1.57079632679489661923f };
	constexpr float PI_DIV_4{ 0.785398163397448309616f };
	constexpr float PI_2{ 6.283185307179586476925f };
	constexpr float PI_4{ 12.56637061435917295385f };

	constexpr float DIV_PI{ 1.f / PI };
	constexpr float DIV_PI_2{ 1.f / PI_2 };
	constexpr float DIV_PI_4{ 1.f / PI_4 };

	constexpr float TO_DEGREES{ 180.f / PI };
	constexpr float TO_RADIANS(PI / 180.0f);

	inline const float Square(float a)
	{
		return a * a;
	}

	inline const float Lerpf(const float a, const float b, const float factor)
	{
		return (1 - factor) * a + (factor * b);
	}

	inline const bool AreEqual(const float a, const float b, const float epsilon = FLT_EPSILON)
	{
		return std::abs(a - b) < epsilon;
	}
	
	inline const bool FloatIsZero(const float flt, const float epsilon = FLT_EPSILON)
	{
		// abs() is heavy
		if (flt < 0.f) return -flt < epsilon;
		else return flt < epsilon;
	}
}