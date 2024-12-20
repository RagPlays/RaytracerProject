#pragma once
#include "DataTypes.h"
#include "Camera.h"

namespace dae
{
	struct Plane;
	struct Sphere;
	struct Light;
	struct Ray;
	struct HitRecord;
	struct Vector3;
	struct Vector4;
	struct ColorRGB;
	struct Camera;

	class Timer;
	class Material;

	//Scene Base Class
	class Scene
	{
	public:
		Scene();
		virtual ~Scene();

		Scene(const Scene&) = delete;
		Scene(Scene&&) noexcept = delete;
		Scene& operator=(const Scene&) = delete;
		Scene& operator=(Scene&&) noexcept = delete;

		virtual void Initialize() = 0;
		virtual void Update(Timer* pTimer);

		Camera& GetCamera();
		const bool GetClosestHit(const Ray& ray, HitRecord& closestHit) const;
		const bool DoesHit(const Ray& ray) const;

		//bool isInsideTriangle(const Vector3& A, const Vector3& B, const Vector3& C, const Vector3& P) const;

		const std::vector<Plane>& GetPlaneGeometries() const;
		const std::vector<Sphere>& GetSphereGeometries() const { return m_SphereGeometries; }
		const std::vector<Light>& GetLights() const { return m_Lights; }
		const std::vector<Material*> GetMaterials() const { return m_Materials; }

	protected:
		std::string	sceneName;

		std::vector<Plane> m_PlaneGeometries;
		std::vector<Sphere> m_SphereGeometries;
		std::vector<TriangleMesh> m_TriangleMeshGeometries;
		std::vector<Light> m_Lights;
		std::vector<Material*> m_Materials;

		Camera m_Camera;

		Sphere* AddSphere(const Vector3& origin, float radius, unsigned char materialIndex = 0);
		Plane* AddPlane(const Vector3& origin, const Vector3& normal, unsigned char materialIndex = 0);
		TriangleMesh* AddTriangleMesh(TriangleCullMode cullMode, unsigned char materialIndex = 0);

		Light* AddPointLight(const Vector3& origin, float intensity, const ColorRGB& color);
		Light* AddDirectionalLight(const Vector3& direction, float intensity, const ColorRGB& color);
		unsigned char AddMaterial(Material* pMaterial);
	};
}
