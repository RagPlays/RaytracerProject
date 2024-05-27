#pragma once

#include "Vector4.h"

namespace dae {

	struct Vector3;
	struct Vector4;

	struct Matrix
	{
		Matrix();
		Matrix(
			const Vector3& xAxis,
			const Vector3& yAxis,
			const Vector3& zAxis,
			const Vector3& t);

		Matrix(
			const Vector4& xAxis,
			const Vector4& yAxis,
			const Vector4& zAxis,
			const Vector4& t);

		Matrix(const Matrix& m);

		const Vector3 TransformVector(const Vector3& v) const;
		const Vector3 TransformVector(const float x, const float y, const float z) const;
		const Vector3 TransformPoint(const Vector3& p) const;
		const Vector3 TransformPoint(const float x, const float y, const float z) const;
		const Matrix& Transpose();

		const Vector3 GetAxisX() const;
		const Vector3 GetAxisY() const;
		const Vector3 GetAxisZ() const;
		const Vector3 GetTranslation() const;

		static const Matrix CreateTranslation(const float x, const float y, const float z);
		static const Matrix CreateTranslation(const Vector3& t);
		static const Matrix CreateRotationX(float pitch);
		static const Matrix CreateRotationY(float yaw);
		static const Matrix CreateRotationZ(float roll);
		static const Matrix CreateRotation(const float pitch, const float yaw, const float roll);
		static const Matrix CreateRotation(const Vector3& r);
		static const Matrix CreateScale(const float sx, const float sy, const float sz);
		static const Matrix CreateScale(const Vector3& s);
		static const Matrix Transpose(Matrix m);

		Vector4& operator[](int index);
		Vector4 operator[](int index) const;
		Matrix operator*(const Matrix& m) const;
		const Matrix& operator*=(const Matrix& m);

	private:

		//Row-Major Matrix
		Vector4 data[4];

		// v0x v0y v0z v0w
		// v1x v1y v1z v1w
		// v2x v2y v2z v2w
		// v3x v3y v3z v3w
	};
}