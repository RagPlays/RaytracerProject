#pragma once
#include "MathHelpers.h"

namespace dae
{
	struct ColorRGB
	{
		ColorRGB() : r{}, g{}, b{}
		{
		}

		ColorRGB(const float rgb) : r{ rgb }, g{ rgb }, b{ rgb }
		{
		}

		ColorRGB(const float _r, const float _g, const float _b) : r{ _r }, g{ _g }, b{ _b }
		{
		}

		float r;
		float g;
		float b;

		void MaxToOne()
		{
			const float maxValue = std::max(r, std::max(g, b));
			if (maxValue > 1.f) *this /= maxValue;
		}

		static const ColorRGB Lerp(const ColorRGB& c1, const ColorRGB& c2, float factor)
		{
			return { Lerpf(c1.r, c2.r, factor), Lerpf(c1.g, c2.g, factor), Lerpf(c1.b, c2.b, factor) };
		}

		#pragma region ColorRGB (Member) Operators

		const ColorRGB& operator+=(const ColorRGB& c)
		{
			r += c.r;
			g += c.g;
			b += c.b;

			return *this;
		}

		const ColorRGB& operator+=(float c)
		{
			r += c;
			g += c;
			b += c;

			return *this;
		}

		const ColorRGB& operator+(const ColorRGB& c)
		{
			return *this += c;
		}

		ColorRGB operator+(const ColorRGB& c) const
		{
			return { r + c.r, g + c.g, b + c.b };
		}

		const ColorRGB& operator-=(const ColorRGB& c)
		{
			r -= c.r;
			g -= c.g;
			b -= c.b;

			return *this;
		}

		const ColorRGB& operator-=(const float c)
		{
			r -= c;
			g -= c;
			b -= c;

			return *this;
		}

		const ColorRGB& operator-(const ColorRGB& c)
		{
			return *this -= c;
		}

		const ColorRGB& operator-(const float c)
		{
			return *this -= c;
		}

		ColorRGB operator-(const ColorRGB& c) const
		{
			return { r - c.r, g - c.g, b - c.b };
		}

		// multiply
		const ColorRGB& operator*=(const ColorRGB& c)
		{
			r *= c.r;
			g *= c.g;
			b *= c.b;

			return *this;
		}

		const ColorRGB& operator*=(float s)
		{
			r *= s;
			g *= s;
			b *= s;

			return *this;
		}

		const ColorRGB& operator*(const ColorRGB& c)
		{
			return *this *= c;
		}

		ColorRGB operator*(const ColorRGB& c) const
		{
			return { r * c.r, g * c.g, b * c.b };
		}

		const ColorRGB& operator*(float s)
		{
			return *this *= s;
		}

		ColorRGB operator*(float s) const
		{
			return { r * s, g * s, b * s };
		}

		// divide 
		const ColorRGB& operator/=(const ColorRGB& c)
		{
			r /= c.r;
			g /= c.g;
			b /= c.b;

			return *this;
		}

		const ColorRGB& operator/=(float s)
		{
			r /= s;
			g /= s;
			b /= s;

			return *this;
		}

		const ColorRGB& operator/(const ColorRGB& c)
		{
			return *this /= c;
		}

		ColorRGB operator/(const ColorRGB& c) const
		{
			return{ r * (1.f / c.r), g * (1.f / c.g), b * (1.f / c.b) };
		}

		const ColorRGB& operator/(float s)
		{
			return *this /= s;
		}

		ColorRGB operator/(float s) const
		{
			return { r / s, g / s, b / s };
		}
		#pragma endregion
	};

	//ColorRGB (Global) Operators
	inline ColorRGB operator*(float s, const ColorRGB& c)
	{
		return c * s;
	}

	inline ColorRGB operator/(float s, const ColorRGB& c)
	{
		return c / s;
	}

	inline ColorRGB operator-(float s, const ColorRGB& c)
	{
		return { 1.f - c.r, 1.f - c.g, 1.f - c.b };
	}

	namespace colors
	{
		static ColorRGB Red{ 1.f,0.f,0.f };
		static ColorRGB Blue{ 0.f,0.f,1.f };
		static ColorRGB Green{ 0.f,1.f,0.f };
		static ColorRGB Yellow{ 1.f,1.f,0.f };
		static ColorRGB Cyan{ 0.f,1.f,1.f };
		static ColorRGB Magenta{ 1.f,0.f,1.f };
		static ColorRGB White{ 1.f,1.f,1.f };
		static ColorRGB Black{ 0.f,0.f,0.f };
		static ColorRGB Gray{ 0.5f,0.5f,0.5f };
	}
}