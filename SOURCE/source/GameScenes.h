#pragma once
#include "Scene.h"

namespace dae
{
	//Forward Declarations
	class Timer;
	class Material;
	struct Plane;
	struct Sphere;
	struct Light;
	struct Triangle;
	struct TriangleMesh;

	class Scene;

	//+++++++++++++++++++++++++++++++++++++++++
	//WEEK 1 Scene
	class Scene_W1 final : public Scene
	{
	public:
		Scene_W1() = default;
		~Scene_W1() override = default;

		Scene_W1(const Scene_W1&) = delete;
		Scene_W1(Scene_W1&&) noexcept = delete;
		Scene_W1& operator=(const Scene_W1&) = delete;
		Scene_W1& operator=(Scene_W1&&) noexcept = delete;

		void Initialize() override;
	};

	//+++++++++++++++++++++++++++++++++++++++++
	//WEEK 2  Scene
	class Scene_W2 final : public Scene
	{
	public:
		Scene_W2() = default;
		~Scene_W2() override = default;

		Scene_W2(const Scene_W2&) = delete;
		Scene_W2(Scene_W2&&) noexcept = delete;
		Scene_W2& operator=(const Scene_W2&) = delete;
		Scene_W2& operator=(Scene_W2&&) noexcept = delete;

		void Initialize() override;
	};

	//+++++++++++++++++++++++++++++++++++++++++
	//WEEK 3 Test Scene
	class Scene_W3_TestScene final : public Scene
	{
	public:
		Scene_W3_TestScene() = default;
		~Scene_W3_TestScene() override = default;

		Scene_W3_TestScene(const Scene_W3_TestScene&) = delete;
		Scene_W3_TestScene(Scene_W3_TestScene&&) noexcept = delete;
		Scene_W3_TestScene& operator=(const Scene_W3_TestScene&) = delete;
		Scene_W3_TestScene& operator=(Scene_W3_TestScene&&) noexcept = delete;

		void Initialize() override;
	};

	//+++++++++++++++++++++++++++++++++++++++++
	//WEEK 3 Scene
	class Scene_W3 final : public Scene
	{
	public:
		Scene_W3() = default;
		~Scene_W3() override = default;

		Scene_W3(const Scene_W3&) = delete;
		Scene_W3(Scene_W3&&) noexcept = delete;
		Scene_W3& operator=(const Scene_W3&) = delete;
		Scene_W3& operator=(Scene_W3&&) noexcept = delete;

		void Initialize() override;
	};

	//+++++++++++++++++++++++++++++++++++++++++
	//WEEK 4 Scene
	class Scene_W4_TestScene final : public Scene
	{
	public:
		Scene_W4_TestScene() = default;
		~Scene_W4_TestScene() override = default;

		Scene_W4_TestScene(const Scene_W4_TestScene&) = delete;
		Scene_W4_TestScene(Scene_W4_TestScene&&) noexcept = delete;
		Scene_W4_TestScene& operator=(const Scene_W4_TestScene&) = delete;
		Scene_W4_TestScene& operator=(Scene_W4_TestScene&&) noexcept = delete;

		void Initialize() override;
		void Update(Timer* pTimer) override;

	private:
		TriangleMesh* m_pMesh{ nullptr };
	};

	//+++++++++++++++++++++++++++++++++++++++++
	//WEEK 4 Scene
	class Scene_W4_ReferenceScene final : public Scene
	{
	public:
		Scene_W4_ReferenceScene() = default;
		~Scene_W4_ReferenceScene() override = default;

		Scene_W4_ReferenceScene(const Scene_W4_ReferenceScene&) = delete;
		Scene_W4_ReferenceScene(Scene_W4_ReferenceScene&&) noexcept = delete;
		Scene_W4_ReferenceScene& operator=(const Scene_W4_ReferenceScene&) = delete;
		Scene_W4_ReferenceScene& operator=(Scene_W4_ReferenceScene&&) noexcept = delete;

		void Initialize() override;
		void Update(Timer* pTimer) override;

	private:
		std::vector<TriangleMesh*> m_Meshes{};
	};

	//+++++++++++++++++++++++++++++++++++++++++
	//WEEK 4 Bunny Scene
	class Scene_W4_BunnyScene final : public Scene
	{
	public:
		Scene_W4_BunnyScene() = default;
		~Scene_W4_BunnyScene() override = default;

		Scene_W4_BunnyScene(const Scene_W4_BunnyScene&) = delete;
		Scene_W4_BunnyScene(Scene_W4_BunnyScene&&) noexcept = delete;
		Scene_W4_BunnyScene& operator=(const Scene_W4_BunnyScene&) = delete;
		Scene_W4_BunnyScene& operator=(Scene_W4_BunnyScene&&) noexcept = delete;

		void Initialize() override;
		void Update(Timer* pTimer) override;
	private:
		TriangleMesh* m_pBunnyMesh{ nullptr };
	};

	//+++++++++++++++++++++++++++++++++++++++++
	//WEEK 4 Bunny Scene
	class Scene_W4_Extra final : public Scene
	{
	public:
		Scene_W4_Extra() = default;
		~Scene_W4_Extra() override = default;

		Scene_W4_Extra(const Scene_W4_Extra&) = delete;
		Scene_W4_Extra(Scene_W4_Extra&&) noexcept = delete;
		Scene_W4_Extra& operator=(const Scene_W4_Extra&) = delete;
		Scene_W4_Extra& operator=(Scene_W4_Extra&&) noexcept = delete;

		void Initialize() override;
		void Update(Timer* pTimer) override;
	private:
		std::vector<TriangleMesh*> m_Meshes{};
	};
}