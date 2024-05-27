#pragma once
#include <cassert>
#include <fstream>
#include "Math.h"
#include "DataTypes.h"

namespace dae
{
	namespace GeometryUtils
	{

#pragma region Sphere HitTest

		/*inline const float FastInvSqrt( float x)	// does accually work -> only it is a approximation so not perfect :(
		{
			const float xhalf = 0.5f * x;
			int i = *(int*)&x;
			i = 0x5f3759df - (i >> 1);
			x = *(float*)&i;
			x = x * (1.5f - xhalf * x * x);
			return x;
		}*/


		//SPHERE HIT-TESTS //
		inline bool HitTest_Sphere(const Sphere& sphere, const Ray& ray, HitRecord& hitRecord)
		{
			const Vector3 tempVec{ ray.origin - sphere.origin };
			const float B{ Vector3::Dot(ray.direction, tempVec) * 2.f };
			const float radiusSquared{ sphere.radius * sphere.radius };
			const float C{ tempVec.SqrMagnitude() - radiusSquared };
			const float D{ B * B - 4.f * C };

			if (D > 0.f)
			{
				const float storeDivide2A{ 0.5f };
				const float sqrtfD{ sqrtf(D) };
				float t{ (-B - sqrtfD) * storeDivide2A };

				if (!(t <= ray.min || t >= ray.max))
				{
					if (t < hitRecord.t)
					{
						hitRecord.didHit = true;
						hitRecord.t = t;
						hitRecord.materialIndex = sphere.materialIndex;
						hitRecord.origin = ray.direction * t + ray.origin;
						hitRecord.normal = (hitRecord.origin - sphere.origin).Normalized();
					}
					return true;
				}

				t = (-B + sqrtfD) * storeDivide2A;

				if (!(t <= ray.min || t >= ray.max))
				{
					if (t < hitRecord.t)
					{
						hitRecord.didHit = true;
						hitRecord.t = t;
						hitRecord.materialIndex = sphere.materialIndex;
						hitRecord.origin = ray.direction * t + ray.origin;
						hitRecord.normal = (hitRecord.origin - sphere.origin).Normalized();
					}
					return true;
				}
			}
			return false;
		}

#pragma endregion
#pragma region Plane HitTest
		//PLANE HIT-TESTS //
		inline bool HitTest_Plane(const Plane& plane, const Ray& ray, HitRecord& hitRecord)
		{
			const float tempDot{ Vector3::Dot(plane.normal, ray.direction) };

			if (FloatIsZero(tempDot)) return false;

			const float t{ Vector3::Dot((plane.origin - ray.origin), plane.normal) / tempDot };

			if (!(t <= ray.min || t >= ray.max))
			{
				if (t < hitRecord.t)
				{
					hitRecord.didHit = true;
					hitRecord.t = t;
					hitRecord.materialIndex = plane.materialIndex;
					hitRecord.origin = ray.direction * t + ray.origin;
					hitRecord.normal = plane.normal;
				}
				return true;
			}
			
			return false;
		}

#pragma endregion

#pragma region TriangeMesh HitTest
		inline bool SlabTest_TriangleMesh(const TriangleMesh& mesh, const Ray& ray)
		{
			// with help of bing chat //
			const Vector3 dirInv{ 1.f / ray.direction };
			float txmin = (mesh.transformedMinAABB.x - ray.origin.x) * dirInv.x;
			float txmax = (mesh.transformedMaxAABB.x - ray.origin.x) * dirInv.x;

			float tymin = (mesh.transformedMinAABB.y - ray.origin.y) * dirInv.y;
			float tymax = (mesh.transformedMaxAABB.y - ray.origin.y) * dirInv.y;

			float tzmin = (mesh.transformedMinAABB.z - ray.origin.z) * dirInv.z;
			float tzmax = (mesh.transformedMaxAABB.z - ray.origin.z) * dirInv.z;

			if (txmin > txmax) {
				const float temp{ txmin };
				txmin = txmax;
				txmax = temp;
			}

			if (tymin > tymax) {
				const float temp{ tymin };
				tymin = tymax;
				tymax = temp;
			}

			if (tzmin > tzmax) {
				const float temp{ tzmin };
				tzmin = tzmax;
				tzmax = temp;
			}

			txmin = (txmin > tymin) ? txmin : tymin;
			txmax = (txmax < tymax) ? txmax : tymax;

			txmin = (txmin > tzmin) ? txmin : tzmin;
			txmax = (txmax < tzmax) ? txmax : tzmax;

			return txmax > 0.f && txmax > txmin;

			// my code
			//float txmin = (mesh.transformedMinAABB.x - ray.origin.x) / ray.direction.x;
			//float txmax = (mesh.transformedMaxAABB.x - ray.origin.x) / ray.direction.x;

			//float tymin = (mesh.transformedMinAABB.y - ray.origin.y) / ray.direction.y;
			//float tymax = (mesh.transformedMaxAABB.y - ray.origin.y) / ray.direction.y;

			//float tzmin = (mesh.transformedMinAABB.z - ray.origin.z) / ray.direction.z;
			//float tzmax = (mesh.transformedMaxAABB.z - ray.origin.z) / ray.direction.z;

			//if (txmin > txmax) std::swap(txmin, txmax); // swap verry fast "bing said is was not :("
			//if (tymin > tymax) std::swap(tymin, tymax);
			//if (tzmin > tzmax) std::swap(tzmin, tzmax);

			//txmin = std::max(txmin, std::max(tymin, tzmin));
			//txmax = std::min(txmax, std::min(tymax, tzmax));

			//return txmax > 0 && txmax > txmin;
		}

		inline bool HitTest_TriangleMesh(const TriangleMesh& mesh, const Ray& ray, HitRecord& hitRecord)
		{
			if (!GeometryUtils::SlabTest_TriangleMesh(mesh, ray)) return false;

			const size_t nrTrianglePoints{ 3 };
			bool returnValue{ false };

			const size_t nrTriangles{ mesh.indices.size() / nrTrianglePoints };

			for (size_t triangleIdx{}; triangleIdx < nrTriangles; ++triangleIdx)
			{
				const Vector3 normal{ mesh.transformedNormals[triangleIdx] };

				const float tempDot{ Vector3::Dot(normal, ray.direction) };

				if (!FloatIsZero(tempDot))  // perpendicular?
				{
					switch (mesh.cullMode)
					{
					case TriangleCullMode::NoCulling:
						break;

					case TriangleCullMode::BackFaceCulling:
						if (!(tempDot < 0.f)) continue;
						break;

					case TriangleCullMode::FrontFaceCulling:
						if (!(tempDot > 0.f)) continue;
						break;
					}

					const Vector3 V0{ mesh.transformedPositions[mesh.indices[triangleIdx * nrTrianglePoints]] };

					const float t{ Vector3::Dot((V0 - ray.origin), normal) / tempDot };

					if (!(t <= ray.min || t >= ray.max))
					{
						const Vector3 V1{ mesh.transformedPositions[mesh.indices[triangleIdx * nrTrianglePoints + 1]] };
						const Vector3 V2{ mesh.transformedPositions[mesh.indices[triangleIdx * nrTrianglePoints + 2]] };

						const Vector3 hitOrigin{ ray.direction * t + ray.origin };

						if (
							(Vector3::Dot(Vector3::Cross(V1 - V0, hitOrigin - V0), normal) < 0.f) ||
							(Vector3::Dot(Vector3::Cross(V2 - V1, hitOrigin - V1), normal) < 0.f) ||
							(Vector3::Dot(Vector3::Cross(V0 - V2, hitOrigin - V2), normal) < 0.f)
							) continue;

						if (t < hitRecord.t)
							{
								hitRecord.didHit = true;
								hitRecord.t = t;
								hitRecord.origin = hitOrigin;
								hitRecord.materialIndex = mesh.materialIndex;
								hitRecord.normal = normal;
							}
						returnValue = true;
					}
				}
			}

			return returnValue;
		}
#pragma endregion
	}

	namespace LightUtils
	{
		//Direction from target to light
		inline Vector3 GetDirectionToLight(const Light& light, const Vector3 origin)
		{
			switch (light.type)
			{
			case LightType::Point:
				return { light.origin - origin };
				break;

			case LightType::Directional:
				return light.direction;
				break;

			default:
				return { light.origin - origin };
				break;
			}
		}

		inline ColorRGB GetRadiance(const Light& light, const Vector3& target)
		{
			switch (light.type)
			{
			case LightType::Point:
				return { light.color * (light.intensity / Vector3{light.origin - target}.SqrMagnitude() ) };
				break;

			case LightType::Directional:
				return { light.color * light.intensity };
				break;

			default:
				return { light.color * (light.intensity / Vector3{light.origin - target}.SqrMagnitude()) };
				break;
			}
		}
	}

	namespace Utils
	{
		//Just parses vertices and indices
#pragma warning(push)
#pragma warning(disable : 4505) //Warning unreferenced local function
		static bool ParseOBJ(const std::string& filename, std::vector<Vector3>& positions, std::vector<Vector3>& normals, std::vector<int>& indices)
		{
			std::ifstream file(filename);
			if (!file)
				return false;

			std::string sCommand;
			// start a while iteration ending when the end of file is reached (ios::eof)
			while (!file.eof())
			{
				//read the first word of the string, use the >> operator (istream::operator>>) 
				file >> sCommand;
				//use conditional statements to process the different commands	
				if (sCommand == "#")
				{
					// Ignore Comment
				}
				else if (sCommand == "v")
				{
					//Vertex
					float x, y, z;
					file >> x >> y >> z;
					positions.push_back({ x, y, z });
				}
				else if (sCommand == "f")
				{
					float i0, i1, i2;
					file >> i0 >> i1 >> i2;

					indices.push_back((int)i0 - 1);
					indices.push_back((int)i1 - 1);
					indices.push_back((int)i2 - 1);
				}
				//read till end of line and ignore all remaining chars
				file.ignore(1000, '\n');

				if (file.eof()) 
					break;
			}

			//Precompute normals
			for (uint64_t index = 0; index < indices.size(); index += 3)
			{
				const uint32_t i0 = indices[index];
				const uint32_t i1 = indices[index + 1];
				const uint32_t i2 = indices[index + 2];

				const Vector3 edgeV0V1 = positions[i1] - positions[i0];
				const Vector3 edgeV0V2 = positions[i2] - positions[i0];
				Vector3 normal{ Vector3::Cross(edgeV0V1, edgeV0V2) };

				if(isnan(normal.x))
				{
					int k = 0;
				}

				normal.Normalize();
				if (isnan(normal.x))
				{
					int k = 0;
				}

				normals.emplace_back(normal);
			}

			return true;
		}
#pragma warning(pop)
	}
}