#pragma once

namespace dae
{
	struct Vector3;

	struct Vector4
	{
		float x;
		float y;
		float z;
		float w;

		Vector4() = default;
		Vector4(const float _x, const float _y, const float _z, const float _w);
		Vector4(const Vector3& v, const float _w);

		const float Magnitude() const;
		const float SqrMagnitude() const;
		const float Normalize();
		const Vector4 Normalized() const;

		static const float Dot(const Vector4& v1, const Vector4& v2);

		// operator overloading
		const Vector4 operator*(float scale) const;
		const Vector4 operator/(float scale) const;
		const Vector4 operator+(const Vector4& v) const;
		const Vector4 operator-(const Vector4& v) const;
		const Vector4& operator+=(const Vector4& v);
		float& operator[](int index);
		float operator[](int index) const;
	};
}
