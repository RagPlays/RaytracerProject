#include <cassert>
#include <cmath>

#include "Matrix.h"
#include "MathHelpers.h"
#include "Vector3.h"
#include "Vector4.h"

namespace dae {

	Matrix::Matrix()
	{
		data[0] = { 1.f, 0.f, 0.f, 0.f }; //xAxis
		data[1] = { 0.f, 1.f, 0.f, 0.f }; //yAxis
		data[2] = { 0.f, 0.f, 1.f, 0.f }; //zAxis
		data[3] = { 0.f, 0.f, 0.f, 1.f };  //T
	}

	Matrix::Matrix(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis, const Vector3& t) :
		Matrix({ xAxis, 0.f }, { yAxis, 0.f }, { zAxis, 0.f }, { t, 1.f })
	{
	}

	Matrix::Matrix(const Vector4& xAxis, const Vector4& yAxis, const Vector4& zAxis, const Vector4& t)
	{
		data[0] = xAxis;
		data[1] = yAxis;
		data[2] = zAxis;
		data[3] = t;
	}

	Matrix::Matrix(const Matrix& m)
	{
		data[0] = m[0];
		data[1] = m[1];
		data[2] = m[2];
		data[3] = m[3];
	}

	const Vector3 Matrix::TransformVector(const Vector3& v) const
	{
		return TransformVector(v[0], v[1], v[2]);
	}

	const Vector3 Matrix::TransformVector(const float x, const float y, const float z) const
	{
		return Vector3
		{
			data[0].x * x + data[1].x * y + data[2].x * z,
			data[0].y * x + data[1].y * y + data[2].y * z,
			data[0].z * x + data[1].z * y + data[2].z * z
		};
	}

	const Vector3 Matrix::TransformPoint(const Vector3& p) const
	{
		return TransformPoint(p[0], p[1], p[2]);
	}

	const Vector3 Matrix::TransformPoint(const float x, const float y, const float z) const
	{
		return Vector3
		{
			data[0].x * x + data[1].x * y + data[2].x * z + data[3].x,
			data[0].y * x + data[1].y * y + data[2].y * z + data[3].y,
			data[0].z * x + data[1].z * y + data[2].z * z + data[3].z,
		};
	}

	const Matrix& Matrix::Transpose()
	{
		const int nrOfVectorsInMatrix{ 4 };
		for (int r{ 0 }; r < nrOfVectorsInMatrix; ++r)
		{
			for (int c{ r + 1 }; c < nrOfVectorsInMatrix; ++c)
			{
				std::swap(data[r][c], data[c][r]);
			}
		}

		return *this;
	}

	const Matrix Matrix::Transpose(Matrix m)
	{
		return m.Transpose();
	}

	const Vector3 Matrix::GetAxisX() const
	{
		return data[0];
	}

	const Vector3 Matrix::GetAxisY() const
	{
		return data[1];
	}

	const Vector3 Matrix::GetAxisZ() const
	{
		return data[2];
	}

	const Vector3 Matrix::GetTranslation() const
	{
		return data[3];
	}

	const Matrix Matrix::CreateTranslation(float x, float y, float z)
	{
		return 
		{ 
			{1.f, 0.f, 0.f, x}, 
			{0.f, 1.f , 0.f, y}, 
			{0.f, 0.f, 1.f, z}, 
			{0.f, 0.f, 0.f, 1.f} 
		};
	}

	const Matrix Matrix::CreateTranslation(const Vector3& t)
	{
		return { Vector3::UnitX, Vector3::UnitY, Vector3::UnitZ, t };
	}

	const Matrix Matrix::CreateRotationX(const float pitch)
	{
		const float cospitch{ cosf(pitch) };
		const float sinpitch{ sinf(pitch) };

		return 
		{ 
			{1.f, 0.f, 0.f, 0.f}, 
			{0.f, cospitch, sinpitch, 0.f}, 
			{0.f, -sinpitch, cospitch, 0.f}, 
			{0.f, 0.f, 0.f, 1.f} 
		};
	}

	const Matrix Matrix::CreateRotationY(const float yaw)
	{
		const float cosyaw{ cosf(yaw) };
		const float sinyaw{ sinf(yaw) };

		return 
		{ 
			{cosyaw, 0.f, -sinyaw, 0.f}, 
			{0.f, 1.f, 0.f, 0.f}, 
			{sinyaw, 0.f, cosyaw, 0.f }, 
			{0.f, 0.f, 0.f, 1.f} 
		};
	}

	const Matrix Matrix::CreateRotationZ(const float roll)
	{
		const float cosroll{ cosf(roll) };
		const float sinroll{ sinf(roll) };

		return 
		{ 
			{cosroll, sinroll, 0.f , 0.f}, 
			{-sinroll, cosroll, 0.f, 0.f},
			{0.f, 0.f, 1.f, 0.f}, 
			{0.f, 0.f, 0.f, 1.f} 
		};
	}

	const Matrix Matrix::CreateRotation(const Vector3& r)
	{
		return { CreateRotationX(r.x) * CreateRotationY(r.y) * CreateRotationZ(r.z) };
	}

	const Matrix Matrix::CreateRotation(const float pitch, const float yaw, const float roll)
	{
		return CreateRotation({ pitch, yaw, roll });
	}

	const Matrix Matrix::CreateScale(const float sx, const float sy, const float sz)
	{
		return 
		{ 
			{sx, 0.f, 0.f, 0.f}, 
			{0.f, sy, 0.f, 0.f}, 
			{0.f, 0.f, sz, 0.f}, 
			{0.f, 0.f, 0.f, 1.f} 
		};
	}

	const Matrix Matrix::CreateScale(const Vector3& s)
	{
		return CreateScale(s[0], s[1], s[2]);
	}

#pragma region Operator Overloads
	Vector4& Matrix::operator[](int index)
	{
		assert(index <= 3 && index >= 0);
		return data[index];
	}

	Vector4 Matrix::operator[](int index) const
	{
		assert(index <= 3 && index >= 0);
		return data[index];
	}

	Matrix Matrix::operator*(const Matrix& m) const
	{
		Matrix result{};
		Matrix m_transposed{ Transpose(m) };

		for (int r{ 0 }; r < 4; ++r)
		{
			for (int c{ 0 }; c < 4; ++c)
			{
				result[r][c] = Vector4::Dot(data[r], m_transposed[c]);
			}
		}

		return result;
	}

	const Matrix& Matrix::operator*=(const Matrix& m)
	{
		Matrix copy{ *this };
		Matrix m_transposed{ Transpose(m) };

		for (int r{ 0 }; r < 4; ++r)
		{
			for (int c{ 0 }; c < 4; ++c)
			{
				data[r][c] = Vector4::Dot(copy[r], m_transposed[c]);
			}
		}

		return *this;
	}
#pragma endregion
}