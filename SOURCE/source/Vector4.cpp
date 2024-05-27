#include "Vector4.h"

#include <cassert>

#include "Vector3.h"
#include <cmath>

namespace dae
{
	Vector4::Vector4(const float _x, const float _y, const float _z, const float _w) : x(_x), y(_y), z(_z), w(_w) 
	{
	}

	Vector4::Vector4(const Vector3& v, const float _w) : x(v.x), y(v.y), z(v.z), w(_w) 
	{
	}

	const float Vector4::Magnitude() const
	{
		return sqrtf(x * x + y * y + z * z + w * w);
	}

	const float Vector4::SqrMagnitude() const
	{
		return x * x + y * y + z * z + w * w;
	}

	const float Vector4::Normalize()
	{
		const float m = Magnitude();
		const float invM{ 1.f / m };
		x *= invM;
		y *= invM;
		z *= invM;
		w *= invM;

		return m;
	}

	const Vector4 Vector4::Normalized() const
	{
		const float invM{ 1.f / Magnitude() };
		return { *this * invM };
	}

	const float Vector4::Dot(const Vector4& v1, const Vector4& v2)
	{
		return { v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w};
	}

#pragma region Operator Overloads
	const Vector4 Vector4::operator*(float scale) const
	{
		return { x * scale, y * scale, z * scale, w * scale };
	}

	const Vector4 Vector4::operator/(float scale) const
	{
		return { x / scale, y / scale, z / scale, w / scale };
	}

	const Vector4 Vector4::operator+(const Vector4& v) const
	{
		return { x + v.x, y + v.y, z + v.z, w + v.w };
	}

	const Vector4 Vector4::operator-(const Vector4& v) const
	{
		return { x - v.x, y - v.y, z - v.z, w - v.w };
	}

	const Vector4& Vector4::operator+=(const Vector4& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}

	float& Vector4::operator[](int index)
	{
		switch (index)
		{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		case 3: return w;
		}

		assert(false);
		return x;
	}

	float Vector4::operator[](int index) const
	{
		switch (index)
		{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		case 3: return w;
		}

		assert(false);
		return x;
	}
#pragma endregion
}