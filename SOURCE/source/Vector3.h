#pragma once

namespace dae
{
	struct Vector4;

	struct Vector3
	{
		float x;
		float y;
		float z;

		Vector3() = default;
		Vector3(const float _x, const float _y, const float _z);
		Vector3(const Vector3& from, const Vector3& to);
		Vector3(const Vector4& v);

		const float Magnitude() const;
		const float SqrMagnitude() const;
		const float Normalize();
		const Vector3 Normalized() const;

		static const float Dot(const Vector3& v1, const Vector3& v2);
		static const Vector3 Cross(const Vector3& v1, const Vector3& v2);
		static const Vector3 Project(const Vector3& v1, const Vector3& v2);
		static const Vector3 Reject(const Vector3& v1, const Vector3& v2);
		static const Vector3 Reflect(const Vector3& v1, const Vector3& v2);
		//static Vector3 Lico(float f1, const Vector3& v1, float f2, const Vector3& v2, float f3, const Vector3& v3);

		static const Vector3 Min(const Vector3& v1, const Vector3& v2);
		static const Vector3 Max(const Vector3& v1, const Vector3& v2);

		const Vector4 ToPoint4() const;
		const Vector4 ToVector4() const;

		//Member Operators
		const Vector3 operator*(float scale) const;
		const Vector3 operator/(float scale) const;
		const Vector3 operator+(const Vector3& v) const;
		const Vector3 operator-(const Vector3& v) const;
		const Vector3 operator-() const;
		//Vector3& operator-();
		Vector3& operator+=(const Vector3& v);
		Vector3& operator-=(const Vector3& v);
		Vector3& operator/=(float scale);
		Vector3& operator*=(float scale);
		float& operator[](int index);
		const float operator[](int index) const;

		static const Vector3 UnitX;
		static const Vector3 UnitY;
		static const Vector3 UnitZ;
		static const Vector3 Zero;
	};

	//Global Operators
	inline const Vector3 operator*(const float scale, const Vector3& v)
	{
		return { v[0] * scale, v[1] * scale, v[2] * scale };
	}

	inline const Vector3 operator/(const float scale, const Vector3& v)
	{
		return { scale / v[0], scale / v[1], scale / v[2] };
	}

	/*inline const Vector3 operator-(const Vector3& v1, const Vector3& v2)
	{
		return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
	}*/
}
