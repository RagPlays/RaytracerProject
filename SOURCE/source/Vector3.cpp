#include <cassert>
#include <cmath>

#include "Vector3.h"
#include "Vector4.h"

namespace dae {
	const Vector3 Vector3::UnitX = Vector3{ 1.f, 0.f, 0.f };
	const Vector3 Vector3::UnitY = Vector3{ 0.f, 1.f, 0.f };
	const Vector3 Vector3::UnitZ = Vector3{ 0.f, 0.f, 1.f };
	const Vector3 Vector3::Zero = Vector3{ 0.f, 0.f, 0.f };

	Vector3::Vector3(const float _x, const float _y, const float _z) : x(_x), y(_y), z(_z)
	{
	}

	Vector3::Vector3(const Vector4& v) : x(v.x), y(v.y), z(v.z)
	{
	}

	Vector3::Vector3(const Vector3& from, const Vector3& to) : x(to.x - from.x), y(to.y - from.y), z(to.z - from.z)
	{
	}

	const float Vector3::Magnitude() const
	{
		return sqrtf(x * x + y * y + z * z);
	}

	const float Vector3::SqrMagnitude() const
	{
		return x * x + y * y + z * z;
	}

	const float Vector3::Normalize()
	{
		const float m = Magnitude();
		const float invM{ 1.f / m };
		x *= invM;
		y *= invM;
		z *= invM;

		return m;
	}

	const Vector3 Vector3::Normalized() const
	{
		const float invM{ 1.f / Magnitude() };
		return { *this * invM };
	}

	const float Vector3::Dot(const Vector3& v1, const Vector3& v2)
	{
		return { v1.x * v2.x + v1.y * v2.y + v1.z * v2.z};
	}

	const Vector3 Vector3::Cross(const Vector3& v1, const Vector3& v2)
	{
		return { v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x };
	}

	const Vector3 Vector3::Project(const Vector3& v1, const Vector3& v2)
	{
		return { v2 * Dot(v1, v2) / Dot(v2, v2) };
	}

	const Vector3 Vector3::Reject(const Vector3& v1, const Vector3& v2)
	{
		return { v1 - v2 * Dot(v1, v2) / Dot(v2, v2) };
	}

	const Vector3 Vector3::Reflect(const Vector3& v1, const Vector3& v2)
	{
		return { v1 - 2.f * Vector3::Dot(v1, v2) * v2 };
	}

	const Vector3 Vector3::Min(const Vector3& v1, const Vector3& v2)
	{
		return
		{
			std::min(v1.x, v2.x),
			std::min(v1.y, v2.y),
			std::min(v1.z, v2.z)
		};
	}

	const Vector3 Vector3::Max(const Vector3& v1, const Vector3& v2)
	{
		return
		{
			std::max(v1.x, v2.x),
			std::max(v1.y, v2.y),
			std::max(v1.z, v2.z)
		};
	}

	const Vector4 Vector3::ToPoint4() const
	{
		return { x, y, z, 1 };
	}

	const Vector4 Vector3::ToVector4() const
	{
		return { x, y, z, 0 };
	}

#pragma region Operator Overloads
	const Vector3 Vector3::operator*(float scale) const
	{
		return { x * scale, y * scale, z * scale };
	}

	const Vector3 Vector3::operator/(float scale) const
	{
		const float invScale{ 1.f / scale };
		return { x * invScale, y * invScale, z * invScale };
	}

	const Vector3 Vector3::operator+(const Vector3& v) const
	{
		return { x + v.x, y + v.y, z + v.z};
	}

	const Vector3 Vector3::operator-(const Vector3& v) const
	{
		return { x - v.x, y - v.y, z - v.z };
	}

	const Vector3 Vector3::operator-() const
	{
		return { -x , -y, -z };
	}

	Vector3& Vector3::operator*=(float scale)
	{
		x *= scale;
		y *= scale;
		z *= scale;
		return *this;
	}

	Vector3& Vector3::operator/=(float scale)
	{
		const float invScale{ 1.f / scale };
		x *= invScale;
		y *= invScale;
		z *= invScale;
		return *this;
	}

	Vector3& Vector3::operator-=(const Vector3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	Vector3& Vector3::operator+=(const Vector3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	float& Vector3::operator[](int index)
	{
		switch (index)
		{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		}

		// cant be right
		assert(false);
		return x;
	}

	const float Vector3::operator[](int index) const
	{
		switch (index)
		{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		}

		// cant be right
		assert(false);
		return x;
	}
#pragma endregion
}