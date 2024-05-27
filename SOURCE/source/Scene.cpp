#include <string>
#include <vector>

#include "Utils.h"
#include "Material.h"

#include "Scene.h"

namespace dae {

#pragma region Base Scene
	//Initialize Scene with Default Solid Color Material (RED)
	Scene::Scene():
		m_Materials({ new Material_SolidColor({1.f,0.f,0.f})})
	{
		m_SphereGeometries.reserve(32);
		m_PlaneGeometries.reserve(32);
		m_TriangleMeshGeometries.reserve(32);
		m_Lights.reserve(32);
		m_Materials.reserve(32);
	}

	Scene::~Scene()
	{
		for(dae::Material*& pMaterial : m_Materials)
		{
			delete pMaterial;
		}
		m_Materials.clear();
	}

	void Scene::Update(Timer* pTimer)
	{
		m_Camera.Update(pTimer);
	}

	Camera& Scene::GetCamera()
	{
		return m_Camera;
	}

	const bool dae::Scene::GetClosestHit(const Ray& ray, HitRecord& closestHit) const
	{	
		// PLANES //
		for (const Plane& plane : m_PlaneGeometries)
		{
			GeometryUtils::HitTest_Plane(plane, ray, closestHit);
		}

		// SPHERES //
		for (const Sphere& sphere : m_SphereGeometries)
		{
			GeometryUtils::HitTest_Sphere(sphere, ray, closestHit);
		}

		// TRIANGLEMESHES //
		for (const TriangleMesh& mesh : m_TriangleMeshGeometries)
		{
			GeometryUtils::HitTest_TriangleMesh(mesh, ray, closestHit);
		}

		return closestHit.didHit;
	}

	const bool Scene::DoesHit(const Ray& ray) const
	{
		const float storeDivide2A{ 0.5f };
		const size_t nrTrianglePoints{ 3 };

		// i did not use the hitTest functions because it calculated more than needed when just trying to figure out if there was a hit // (so i dont use ignoreHitRecord)

		// PLANES
		for (const Plane& plane : m_PlaneGeometries)
		{
			const float tempDot{ Vector3::Dot(plane.normal, ray.direction) };

			if (!FloatIsZero(tempDot))
			{
				const float t{ Vector3::Dot(plane.origin - ray.origin, plane.normal) / tempDot };

				if (!(t <= ray.min || t >= ray.max)) return true;
			}
		}

		// SPHERES
		for (const Sphere& sphere : m_SphereGeometries)
		{
			const Vector3 tempVec{ ray.origin - sphere.origin };
			const float B{ Vector3::Dot(ray.direction, tempVec) * 2.f };
			const float radiusSquared{ sphere.radius * sphere.radius };
			const float C{ tempVec.SqrMagnitude() - radiusSquared };
			const float D{ B * B - 4.f * C };

			if (D > 0.f)
			{
				const float sqrtfD{ sqrtf(D) };
				const float t{ (-B - sqrtfD) * storeDivide2A };

				if (!(t <= ray.min || t >= ray.max)) return true;

				// no need to check when inside sphere because you dont even see the shadows
			}
		}

		// TRIANGLEMESHES
		for (const TriangleMesh& mesh : m_TriangleMeshGeometries)
		{
			if (GeometryUtils::SlabTest_TriangleMesh(mesh, ray))
			{
				const size_t nrTriangles{ mesh.indices.size() / nrTrianglePoints };

				for (size_t TriangleIdx{}; TriangleIdx < nrTriangles; ++TriangleIdx)
				{
					const Vector3 normal{ mesh.transformedNormals[TriangleIdx] };

					const float tempDot{ Vector3::Dot(normal, ray.direction) };

					if (!FloatIsZero(tempDot))  // not perpendicular
					{
						switch (mesh.cullMode)
						{
						case TriangleCullMode::NoCulling:
							break;

						case TriangleCullMode::BackFaceCulling:
							if (!(tempDot > 0.f)) continue;
							break;

						case TriangleCullMode::FrontFaceCulling:
							if (!(tempDot < 0.f)) continue;
							break;
						}

						const Vector3 V0{ mesh.transformedPositions[mesh.indices[TriangleIdx * nrTrianglePoints]] };

						const float t{ Vector3::Dot(V0 - ray.origin, normal) / tempDot };

						if (!(t <= ray.min || t >= ray.max))
						{
							const Vector3 V1{ mesh.transformedPositions[mesh.indices[TriangleIdx * nrTrianglePoints + 1]] };
							const Vector3 V2{ mesh.transformedPositions[mesh.indices[TriangleIdx * nrTrianglePoints + 2]] };

							const Vector3 hitOrigin{ ray.direction * t + ray.origin };

							if (
								(Vector3::Dot(Vector3::Cross(V1 - V0, hitOrigin - V0), normal) < 0.f) ||
								(Vector3::Dot(Vector3::Cross(V2 - V1, hitOrigin - V1), normal) < 0.f) ||
								(Vector3::Dot(Vector3::Cross(V0 - V2, hitOrigin - V2), normal) < 0.f)
								) continue;

							return true;

							// MOLLER_TRUMBORE but less fps and noise in shadows
							/*const Vector3 AB{ V1 - V0 };
							const Vector3 AC{ V2 - V0 };
							const float areaABC{ 0.5f * Vector3::Cross(AB, AC).Magnitude() };

							const Vector3 AP{ hitOrigin - V0 };
							const Vector3 BP{ hitOrigin - V1 };
							const Vector3 CP{ hitOrigin - V2 };

							const float areaABP{ 0.5f * Vector3::Cross(AB, BP).Magnitude() };
							const float areaACP{ 0.5f * Vector3::Cross(AC, CP).Magnitude() };
							const float areaABCP{ areaABC };

							const float a{ areaABP / areaABCP };
							const float b{ areaACP / areaABCP };
							const float c{ 1.0f - a - b };

							if (a < 0.f || b < 0.f || c < 0.f || a + b + c > 1.f) continue;

							return true;*/

						}
					}
				}
			}
		}
			
		return false;
	}

	const std::vector<Plane>& Scene::GetPlaneGeometries() const
	{
		return m_PlaneGeometries;
	}

#pragma region Scene Helpers
	Sphere* Scene::AddSphere(const Vector3& origin, float radius, unsigned char materialIndex)
	{
		m_SphereGeometries.emplace_back(origin, radius, materialIndex);
		return &m_SphereGeometries.back();
	}

	Plane* Scene::AddPlane(const Vector3& origin, const Vector3& normal, unsigned char materialIndex)
	{
		m_PlaneGeometries.emplace_back(origin, normal, materialIndex);
		return &m_PlaneGeometries.back();
	}

	TriangleMesh* Scene::AddTriangleMesh(TriangleCullMode cullMode, unsigned char materialIndex)
	{
		m_TriangleMeshGeometries.emplace_back(cullMode, materialIndex);
		return &m_TriangleMeshGeometries.back();
	}

	Light* Scene::AddPointLight(const Vector3& origin, float intensity, const ColorRGB& color)
	{
		m_Lights.emplace_back(origin, intensity, color, LightType::Point);
		return &m_Lights.back();
	}

	Light* Scene::AddDirectionalLight(const Vector3& direction, float intensity, const ColorRGB& color)
	{
		m_Lights.emplace_back(direction, intensity, color);
		return &m_Lights.back();
	}

	unsigned char Scene::AddMaterial(Material* pMaterial)
	{
		m_Materials.emplace_back(pMaterial);
		return static_cast<unsigned char>(m_Materials.size() - 1);
	}
#pragma endregion
#pragma endregion
}