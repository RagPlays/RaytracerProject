#pragma once
#include "Math.h"
#include "vector"

namespace dae
{
#pragma region GEOMETRY
	struct Sphere
	{
		Sphere() : origin{}, radius{}, materialIndex{ 0 }
		{
		}

		Sphere(const Vector3& _origin, const float _radius, const unsigned char _matIdx) 
			: origin{ _origin }, radius{ _radius }, materialIndex{ _matIdx }
		{
		}

		Vector3 origin;
		float radius;

		unsigned char materialIndex;
	};

	struct Plane
	{
		Plane() : origin{}, normal{}, materialIndex{ 0 }
		{
		}

		Plane(const Vector3& _origin, const Vector3& _normal, const unsigned char _matIdx)
			: origin{ _origin }, normal{ _normal }, materialIndex{ _matIdx }
		{
		}

		Vector3 origin;
		Vector3 normal;

		unsigned char materialIndex;
	};

	enum class TriangleCullMode
	{
		FrontFaceCulling,
		BackFaceCulling,
		NoCulling
	};

	struct Triangle
	{
		Triangle() = default;

		Triangle(const Vector3& _v0, const Vector3& _v1, const Vector3& _v2, const Vector3& _normal):
			v0{_v0}, v1{_v1}, v2{_v2}, normal{_normal.Normalized()}
		{
		}

		Triangle(const Vector3& _v0, const Vector3& _v1, const Vector3& _v2) :
			v0{ _v0 }, v1{ _v1 }, v2{ _v2 }
		{
			normal = Vector3::Cross(v1 - v0, v2 - v0).Normalized();
		}

		Vector3 v0{};
		Vector3 v1{};
		Vector3 v2{};

		Vector3 normal{};

		TriangleCullMode cullMode{};
		unsigned char materialIndex{};
	};

	struct TriangleMesh
	{
		TriangleMesh()
		{
			positions.reserve(30);
			normals.reserve(10);
			indices.reserve(30);
		}

		TriangleMesh(TriangleCullMode _cullMode, const unsigned char _matIdx)
			: cullMode{ _cullMode }, materialIndex{ _matIdx }
		{
			positions.reserve(30);
			normals.reserve(10);
			indices.reserve(30);
		}

		TriangleMesh(const std::vector<Vector3>& _positions, const std::vector<int>& _indices, TriangleCullMode _cullMode = TriangleCullMode::NoCulling):
		positions(_positions), indices(_indices), cullMode(_cullMode)
		{
			CalculateNormals();

			UpdateTransforms();
		}

		TriangleMesh(const std::vector<Vector3>& _positions, const std::vector<int>& _indices, const std::vector<Vector3>& _normals, TriangleCullMode _cullMode = TriangleCullMode::NoCulling) :
			positions(_positions), indices(_indices), normals(_normals), cullMode(_cullMode)
		{
			UpdateTransforms();
		}

		std::vector<Vector3> positions{};
		std::vector<Vector3> normals{};
		std::vector<int> indices{};
		unsigned char materialIndex{};

		TriangleCullMode cullMode{};

		Matrix rotationTransform{};
		Matrix translationTransform{};
		Matrix scaleTransform{};

		Vector3 minAABB{};
		Vector3 maxAABB{};

		Vector3 transformedMinAABB{};
		Vector3 transformedMaxAABB{};

		std::vector<Vector3> transformedPositions{};
		std::vector<Vector3> transformedNormals{};

		void Translate(const Vector3& translation)
		{
			translationTransform = Matrix::CreateTranslation(translation);
		}

		void RotateY(const float yaw)
		{
			rotationTransform = Matrix::CreateRotationY(yaw);
		}

		void Scale(const Vector3& scale)
		{
			scaleTransform = Matrix::CreateScale(scale);
		}

		void AppendTriangle(const Triangle& triangle, const bool ignoreTransformUpdate = false)
		{
			int startIndex = static_cast<int>(positions.size());

			positions.emplace_back(triangle.v0);
			positions.emplace_back(triangle.v1);
			positions.emplace_back(triangle.v2);

			indices.emplace_back(startIndex);
			indices.emplace_back(++startIndex);
			indices.emplace_back(++startIndex);

			normals.emplace_back(triangle.normal);

			//Not ideal, but making sure all vertices are updated
			if(!ignoreTransformUpdate) UpdateTransforms();
		}

		void CalculateNormals()
		{
			const size_t nrTrianglePoints{ 3 };
			const size_t nrTriangles{ indices.size() / nrTrianglePoints };

			normals.resize(nrTriangles);

			for (size_t triangleIdx{}; triangleIdx < nrTriangles; ++triangleIdx)
			{
				const size_t baseIdx{ triangleIdx * nrTrianglePoints };

				const Vector3& P0 = positions[indices[baseIdx]];
				const Vector3& P1 = positions[indices[baseIdx + 1]];
				const Vector3& P2 = positions[indices[baseIdx + 2]];

				normals[triangleIdx] = Vector3::Cross(P1 - P0, P2 - P0).Normalized();
			}
		}

		void UpdateTransforms()
		{
			const Matrix finalTransform{ scaleTransform * rotationTransform * translationTransform };

			//Transform Positions (positions > transformedPositions)
			transformedPositions.resize(positions.size());
			for (size_t idx{}; idx < positions.size(); ++idx)
			{
				transformedPositions[idx] = finalTransform.TransformPoint(positions[idx]);
			}

			//Transform Normals (normals > transformedNormals)
			transformedNormals.resize(normals.size());
			for (size_t idx{}; idx < normals.size(); ++idx)
			{
				transformedNormals[idx] = finalTransform.TransformVector(normals[idx]);
			}

			// Update AABB
			UpdateTransformedAABB(finalTransform);
		}


		void UpdateAABB()
		{
			if (positions.empty())
			{
				return;
			}

			minAABB = positions[0];
			maxAABB = positions[0];

			for (const auto& p : positions)
			{
				minAABB = Vector3::Min(p, minAABB);
				maxAABB = Vector3::Max(p, maxAABB);
			}
		}

		void UpdateTransformedAABB(const Matrix& finalTransform)
		{
			// AABB update: be careful -> transform the 8 vertices of the aabb
			// and calculate new min and max.
			Vector3 tMinAABB = finalTransform.TransformPoint(minAABB);
			Vector3 tMaxAABB = tMinAABB;
			Vector3 tAABB;
			// (xmax, ymin, zmin)
			tAABB = finalTransform.TransformPoint(maxAABB.x, minAABB.y, minAABB.z);
			tMinAABB = Vector3::Min(tAABB, tMinAABB);
			tMaxAABB = Vector3::Max(tAABB, tMaxAABB);
			// (xmax, ymin, zmax)
			tAABB = finalTransform.TransformPoint(maxAABB.x, minAABB.y, maxAABB.z);
			tMinAABB = Vector3::Min(tAABB, tMinAABB);
			tMaxAABB = Vector3::Max(tAABB, tMaxAABB);
			// (xmin, ymin, zmax)
			tAABB = finalTransform.TransformPoint(minAABB.x, minAABB.y, maxAABB.z);
			tMinAABB = Vector3::Min(tAABB, tMinAABB);
			tMaxAABB = Vector3::Max(tAABB, tMaxAABB);
			// (xmin, ymax, zmin)
			tAABB = finalTransform.TransformPoint(minAABB.x, maxAABB.y, minAABB.z);
			tMinAABB = Vector3::Min(tAABB, tMinAABB);
			tMaxAABB = Vector3::Max(tAABB, tMaxAABB);
			// (xmax, ymax, zmin)
			tAABB = finalTransform.TransformPoint(maxAABB.x, maxAABB.y, minAABB.z);
			tMinAABB = Vector3::Min(tAABB, tMinAABB);
			tMaxAABB = Vector3::Max(tAABB, tMaxAABB);
			// (xmax, ymax, zmax)
			tAABB = finalTransform.TransformPoint(maxAABB.x, maxAABB.y, maxAABB.z);
			tMinAABB = Vector3::Min(tAABB, tMinAABB);
			tMaxAABB = Vector3::Max(tAABB, tMaxAABB);
			// (xmin, ymax, zmax)
			tAABB = finalTransform.TransformPoint(minAABB.x, maxAABB.y, maxAABB.z);
			tMinAABB = Vector3::Min(tAABB, tMinAABB);
			tMaxAABB = Vector3::Max(tAABB, tMaxAABB);

			transformedMinAABB = tMinAABB;
			transformedMaxAABB = tMaxAABB;
		}
	};
#pragma endregion
#pragma region LIGHT
	enum class LightType
	{
		Point = 0,
		Directional,
		Ambient,
		Spot,
		Area
	};

	struct Light
	{
		// Point light
		Light(const Vector3& _origin, const float _intensity, const ColorRGB& _color, const LightType _type)
			: origin{ _origin }, intensity{ _intensity }, color{ _color }, type{ _type }
		{
		}

		// Directional light
		Light(const Vector3& _direction, const float _intensity, const ColorRGB& _color)
			: direction{ _direction }, intensity{ _intensity }, color{ _color }, type{ LightType::Directional }
		{
		}

		Vector3 origin{};
		Vector3 direction{};
		ColorRGB color;
		float intensity;

		LightType type;
	};

#pragma endregion
#pragma region MISC
	struct Ray
	{
		Vector3 origin{};
		Vector3 direction{};

		float min{ 0.0001f };
		float max{ FLT_MAX };
	};

	struct HitRecord
	{
		Vector3 origin{};
		Vector3 normal{};
		float t{ FLT_MAX };

		bool didHit{ false };
		unsigned char materialIndex{ 0 };
	};
#pragma endregion
}