#include <iostream>
#include "GameScenes.h"

#include "Utils.h"
#include "Material.h"

namespace dae
{
	void Scene_W1::Initialize()
	{
		sceneName = "scene week 1";
		m_Camera.origin = { 0.f, 0.f, 0.f };
		m_Camera.fovAngle = 45;

		//default: Material id0 >> SolidColor Material (RED)
		constexpr unsigned char matId_Solid_Red = 0;
		const unsigned char matId_Solid_Blue = AddMaterial(new Material_SolidColor{ colors::Blue });

		const unsigned char matId_Solid_Yellow = AddMaterial(new Material_SolidColor{ colors::Yellow });
		const unsigned char matId_Solid_Green = AddMaterial(new Material_SolidColor{ colors::Green });
		const unsigned char matId_Solid_Magenta = AddMaterial(new Material_SolidColor{ colors::Magenta });

		//Spheres
		AddSphere({ -25.f, 0.f, 100.f }, 50.f, matId_Solid_Red);
		AddSphere({ 25.f, 0.f, 100.f }, 50.f, matId_Solid_Blue);

		//Plane
		AddPlane({ -75.f, 0.f, 0.f }, { 1.f, 0.f,0.f }, matId_Solid_Green);
		AddPlane({ 75.f, 0.f, 0.f }, { -1.f, 0.f,0.f }, matId_Solid_Green);
		AddPlane({ 0.f, -75.f, 0.f }, { 0.f, 1.f,0.f }, matId_Solid_Yellow);
		AddPlane({ 0.f, 75.f, 0.f }, { 0.f, -1.f,0.f }, matId_Solid_Yellow);
		AddPlane({ 0.f, 0.f, 125.f }, { 0.f, 0.f,-1.f }, matId_Solid_Magenta);


		// Some test dot and cross product //
		// DOT //
		std::cout << (Vector3::Dot(Vector3::UnitX, Vector3::UnitX)) << "\n";
		std::cout << (Vector3::Dot(Vector3::UnitX, -Vector3::UnitX)) << "\n";
		std::cout << (Vector3::Dot(Vector3::UnitX, Vector3::UnitY)) << "\n";

		// CROSS //
		Vector3 crossResult{}; // Left-Handed!
		crossResult = Vector3::Cross(Vector3::UnitZ, Vector3::UnitX);
		std::cout << "Cross Product 1: (" << crossResult.x << ", " << crossResult.y << ", " << crossResult.z << ")\n";
		crossResult = Vector3::Cross(Vector3::UnitX, Vector3::UnitZ);
		std::cout << "Cross Product 2: (" << crossResult.x << ", " << crossResult.y << ", " << crossResult.z << ")\n";
		std::cout << "\n";
	}

	void Scene_W2::Initialize()
	{
		sceneName = "scene week 2";
		m_Camera.origin = { 0.f, 3.f, -9.f };
		m_Camera.fovAngle = 45;
		m_Camera.fovValue = tanf((m_Camera.fovAngle * TO_RADIANS) * 0.5f);

		constexpr unsigned char matId_Solid_Red = 0;

		const unsigned char matId_Solid_Blue = AddMaterial(new Material_SolidColor{ colors::Blue });
		const unsigned char matId_Solid_Yellow = AddMaterial(new Material_SolidColor{ colors::Yellow });
		const unsigned char matId_Solid_Green = AddMaterial(new Material_SolidColor{ colors::Green });
		const unsigned char matId_Solid_Magenta = AddMaterial(new Material_SolidColor{ colors::Magenta });

		// planes
		AddPlane(Vector3{ 0.f, 0.f, 10.f }, Vector3{ 0.f, 0.f, -1.f }, matId_Solid_Magenta); //BACK
		AddPlane(Vector3{ 0.f, 0.f, 0.f }, Vector3{ 0.f, 1.f, 0.f }, matId_Solid_Yellow); //BOTTOM
		AddPlane(Vector3{ 0.f, 10.f, 0.f }, Vector3{ 0.f, -1.f, 0.f }, matId_Solid_Yellow); //TOP
		AddPlane(Vector3{ 5.f, 0.f, 0.f }, Vector3{ -1.f, 0.f, 0.f }, matId_Solid_Green); //RIGHT
		AddPlane(Vector3{ -5.f, 0.f, 0.f }, Vector3{ 1.f, 0.f, 0.f }, matId_Solid_Green); //LEFT

		// spheres
		AddSphere(Vector3{ -1.75f, 1.f, 0.f }, .75f, matId_Solid_Red);
		AddSphere(Vector3{ 0.f, 1.f, 0.f }, .75f, matId_Solid_Blue);
		AddSphere(Vector3{ 1.75f, 1.f, 0.f }, .75f, matId_Solid_Red);
		AddSphere(Vector3{ -1.75f, 3.f, 0.f }, .75f, matId_Solid_Blue);
		AddSphere(Vector3{ 0.f, 3.f, 0.f }, .75f, matId_Solid_Red);
		AddSphere(Vector3{ 1.75f, 3.f, 0.f }, .75f, matId_Solid_Blue);

		AddPointLight(Vector3{ 0.f, 5.f, -5.f }, 70.f, colors::White); //Backlight
	}

	void Scene_W3_TestScene::Initialize()
	{
		sceneName = "test scene week 3";
		m_Camera.origin = { 0.f, 1.f, -5.f };
		m_Camera.fovAngle = 45;
		m_Camera.fovValue = tanf((m_Camera.fovAngle * TO_RADIANS) * 0.5f);

		//default: Material id0 >> SolidColor Material (RED)
		const unsigned char matLambert_Red = AddMaterial(new Material_Lambert{ colors::Red, 1.f });
		const unsigned char matLambert_Blue = AddMaterial(new Material_Lambert{ colors::Blue, 1.f });
		const unsigned char matLambert_Yellow = AddMaterial(new Material_Lambert{ colors::Yellow, 1.f });

		const unsigned char matLambertPhong_Blue = AddMaterial(new Material_LambertPhong{ colors::Blue, 1.f, 1.f, 60.f });

		//Spheres
		AddSphere({ -.75f, 1.f, .0f }, 1.f, matLambertPhong_Blue);
		AddSphere({ .75f, 1.f, .0f }, 1.f, matLambert_Blue);

		//Planes
		AddPlane({ 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f }, matLambert_Yellow);

		//Lights
		AddPointLight({ 0.f, 5.f, 5.f }, 25.f, colors::White);
		AddPointLight({ 0.f, 2.5f, -5.f }, 25.f, colors::White);
	}

	void Scene_W3::Initialize()
	{
		sceneName = "Scene Week 3";
		m_Camera.origin = { 0.f, 3.f, -9.f };
		m_Camera.fovAngle = 45;
		m_Camera.fovValue = tanf((m_Camera.fovAngle * TO_RADIANS) * 0.5f);

		const unsigned char matCT_GrayRoughMetal = AddMaterial(new Material_CookTorrence({ .972f, .960f, .915f }, 1.f, 1.f));
		const unsigned char matCT_GrayMediumMetal = AddMaterial(new Material_CookTorrence({ .972f, .960f, .915f }, 1.f, .6f));
		const unsigned char matCT_GraySmoothMetal = AddMaterial(new Material_CookTorrence({ .972f, .960f, .915f }, 1.f, .1f));
		const unsigned char matCT_GrayRoughPlastic = AddMaterial(new Material_CookTorrence({ .75f, .75f, .75f }, .0f, 1.f));
		const unsigned char matCT_GrayMediumPlastic = AddMaterial(new Material_CookTorrence({ .75f, .75f, .75f }, .0f, .6f));
		const unsigned char matCT_GraySmoothPlastic = AddMaterial(new Material_CookTorrence({ .75f, .75f, .75f }, .0f, .1f));

		const unsigned char matLambert_GrayBlue = AddMaterial(new Material_Lambert({ .49f, 0.57f, 0.57f }, 1.f));
		const unsigned char matLambert_White = AddMaterial(new Material_Lambert(colors::White, 1.f));

		// Planes
		AddPlane(Vector3{ 0.f, 0.f, 10.f }, Vector3{ 0.f, 0.f, -1.f }, matLambert_GrayBlue); //BACK
		AddPlane(Vector3{ 0.f, 0.f, 0.f }, Vector3{ 0.f, 1.f, 0.f }, matLambert_GrayBlue); //BOTTOM
		AddPlane(Vector3{ 0.f, 10.f, 0.f }, Vector3{ 0.f, -1.f, 0.f }, matLambert_GrayBlue); //TOP
		AddPlane(Vector3{ 5.f, 0.f, 0.f }, Vector3{ -1.f, 0.f, 0.f }, matLambert_GrayBlue); //RIGHT
		AddPlane(Vector3{ -5.f, 0.f, 0.f }, Vector3{ 1.f, 0.f, 0.f }, matLambert_GrayBlue); //LEFT

		// temp spheres
		const unsigned char mathLambertPhong1 = AddMaterial(new Material_LambertPhong(colors::Blue, 0.5f, 0.5f, 3.f));
		const unsigned char mathLambertPhong2 = AddMaterial(new Material_LambertPhong(colors::Blue, 0.5f, 0.5f, 15.f));
		const unsigned char mathLambertPhong3 = AddMaterial(new Material_LambertPhong(colors::Blue, 0.5f, 0.5f, 50.f));

		// temp Spheres
		/*AddSphere(Vector3{ -1.75f, 1.f, 0.f }, .75f, mathLambertPhong1);
		AddSphere(Vector3{ 0.f, 1.f, 0.f }, .75f, mathLambertPhong2);
		AddSphere(Vector3{ 1.75f, 1.f, 0.f }, .75f, mathLambertPhong3);*/

		// Spheres
		AddSphere(Vector3{ -1.75f, 1.f, 0.f }, .75f, matCT_GrayRoughMetal);
		AddSphere(Vector3{ 0.f, 1.f, 0.f }, .75f, matCT_GrayMediumMetal);
		AddSphere(Vector3{ 1.75f, 1.f, 0.f }, .75f, matCT_GraySmoothMetal);
		AddSphere(Vector3{ -1.75f, 3.f, 0.f }, .75f, matCT_GrayRoughPlastic);
		AddSphere(Vector3{ 0.f, 3.f, 0.f }, .75f, matCT_GrayMediumPlastic);
		AddSphere(Vector3{ 1.75f, 3.f, 0.f }, .75f, matCT_GraySmoothPlastic);

		// Light
		AddPointLight(Vector3{ 0.f, 5.f, 5.f }, 50.f, ColorRGB{ 1.f, .61f, .45f }); //Backlight
		AddPointLight(Vector3{ -2.5f, 5.f, -5.f }, 70.f, ColorRGB{ 1.f, .8f, .45f }); //Front Light Left
		AddPointLight(Vector3{ 2.5f, 2.5f, -5.f }, 50.f, ColorRGB{ .34f, .47f, .68f });
	}

	void Scene_W4_TestScene::Initialize()
	{
		sceneName = "test scene week 4";
		m_Camera.origin = { 0.f,1.f, -5.f };
		m_Camera.totalYaw = 0.f;
		m_Camera.fovAngle = 45;
		m_Camera.fovValue = tanf((m_Camera.fovAngle * TO_RADIANS) * 0.5f);

		//Materials
		const unsigned char matLambert_GrayBlue = AddMaterial(new Material_Lambert({ .49f, 0.57f, 0.57f }, 1.f));
		const unsigned char matLambert_White = AddMaterial(new Material_Lambert(colors::White, 1.f));

		//Planes
		AddPlane(Vector3{ 0.f, 0.f, 10.f }, Vector3{ 0.f, 0.f, -1.f }, matLambert_GrayBlue); //BACK
		AddPlane(Vector3{ 0.f, 0.f, 0.f }, Vector3{ 0.f, 1.f, 0.f }, matLambert_GrayBlue); //BOTTOM
		AddPlane(Vector3{ 0.f, 10.f, 0.f }, Vector3{ 0.f, -1.f, 0.f }, matLambert_GrayBlue); //TOP
		AddPlane(Vector3{ 5.f, 0.f, 0.f }, Vector3{ -1.f, 0.f, 0.f }, matLambert_GrayBlue); //RIGHT
		AddPlane(Vector3{ -5.f, 0.f, 0.f }, Vector3{ 1.f, 0.f, 0.f }, matLambert_GrayBlue); //LEFT

		////Triangle (Temp)
		////===============
		/*Triangle triangle = Triangle{ {-.75f,.5f,.0f},{-.75f,2.f, .0f}, {.75f,.5f,0.f} };
		triangle.cullMode = TriangleCullMode::NoCulling;
		triangle.materialIndex = matLambert_White;

		m_Triangles.emplace_back(triangle);*/

		//Triangle Mesh
		//=============
		//m_pMesh = AddTriangleMesh(TriangleCullMode::NoCulling, matLambert_White);
		//m_pMesh->positions = {
		//	{-.75f,-1.f,.0f},  //V0
		//	{-.75f,1.f, .0f},  //V2
		//	{.75f,1.f,1.f},    //V3
		//	{.75f,-1.f,0.f} }; //V4

		//m_pMesh->indices = {
		//	0,1,2, //Triangle 1
		//	0,2,3  //Triangle 2
		//};

		//m_pMesh->CalculateNormals();

		//m_pMesh->Translate({ 0.f,1.5f,0.f });
		//m_pMesh->RotateY(180);

		//m_pMesh->UpdateAABB();
		//m_pMesh->UpdateTransforms();

		////OBJ
		////===
		m_pMesh = AddTriangleMesh(TriangleCullMode::BackFaceCulling, matLambert_White);
		Utils::ParseOBJ(
			//"Resources/simple_quad.obj",
			//"Resources/simple_cube.obj",
			"Resources/simple_object.obj",
			//"Resources/lowpoly_bunny.obj",
			m_pMesh->positions,
			m_pMesh->normals,
			m_pMesh->indices);

		m_pMesh->Scale({ .7f,.7f,.7f });
		m_pMesh->Translate({ .0f,1.f,0.f });

		//No need to Calculate the normals, these are calculated inside the ParseOBJ function
		m_pMesh->UpdateAABB();
		m_pMesh->UpdateTransforms();


		//Light
		AddPointLight(Vector3{ 0.f, 5.f, 5.f }, 50.f, ColorRGB{ 1.f, .61f, .45f }); //Backlight
		AddPointLight(Vector3{ -2.5f, 5.f, -5.f }, 70.f, ColorRGB{ 1.f, .8f, .45f }); //Front Light Left
		AddPointLight(Vector3{ 2.5f, 2.5f, -5.f }, 50.f, ColorRGB{ .34f, .47f, .68f });
	}

	void Scene_W4_TestScene::Update(Timer* pTimer)
	{
		Scene::Update(pTimer);

		if (m_pMesh)
		{
			m_pMesh->RotateY(PI_DIV_2 * pTimer->GetTotal());
			m_pMesh->UpdateTransforms();
		}
	}

	void Scene_W4_ReferenceScene::Initialize()
	{
		sceneName = "reference scene week 4";
		m_Camera.origin = { 0.f, 3.f, -9.f };
		m_Camera.fovAngle = 45;
		m_Camera.fovValue = tanf((m_Camera.fovAngle * TO_RADIANS) * 0.5f);

		const unsigned char matCT_GrayRoughMetal = AddMaterial(new Material_CookTorrence({ .972f, .960f, .915f }, 1.f, 1.f));
		const unsigned char matCT_GrayMediumMetal = AddMaterial(new Material_CookTorrence({ .972f, .960f, .915f }, 1.f, .6f));
		const unsigned char matCT_GraySmoothMetal = AddMaterial(new Material_CookTorrence({ .972f, .960f, .915f }, 1.f, .1f));
		const unsigned char matCT_GrayRoughPlastic = AddMaterial(new Material_CookTorrence({ .75f, .75f, .75f }, .0f, 1.f));
		const unsigned char matCT_GrayMediumPlastic = AddMaterial(new Material_CookTorrence({ .75f, .75f, .75f }, .0f, .6f));
		const unsigned char matCT_GraySmoothPlastic = AddMaterial(new Material_CookTorrence({ .75f, .75f, .75f }, .0f, .1f));

		const unsigned char matLambert_GrayBlue = AddMaterial(new Material_Lambert({ .49f, 0.57f, 0.57f }, 1.f));
		const unsigned char matLambert_White = AddMaterial(new Material_Lambert(colors::White, 0.6f));

		AddPlane(Vector3{ 0.f, 0.f, 10.f }, Vector3{ 0.f, 0.f, -1.f }, matLambert_GrayBlue); //BACK
		AddPlane(Vector3{ 0.f, 0.f, 0.f }, Vector3{ 0.f, 1.f, 0.f }, matLambert_GrayBlue); //BOTTOM
		AddPlane(Vector3{ 0.f, 10.f, 0.f }, Vector3{ 0.f, -1.f, 0.f }, matLambert_GrayBlue); //TOP
		AddPlane(Vector3{ 5.f, 0.f, 0.f }, Vector3{ -1.f, 0.f, 0.f }, matLambert_GrayBlue); //RIGHT
		AddPlane(Vector3{ -5.f, 0.f, 0.f }, Vector3{ 1.f, 0.f, 0.f }, matLambert_GrayBlue); //LEFT

		AddSphere(Vector3{ -1.75f, 1.f, 0.f }, .75f, matCT_GrayRoughMetal);
		AddSphere(Vector3{ 0.f, 1.f, 0.f }, .75f, matCT_GrayMediumMetal);
		AddSphere(Vector3{ 1.75f, 1.f, 0.f }, .75f, matCT_GraySmoothMetal);
		AddSphere(Vector3{ -1.75f, 3.f, 0.f }, .75f, matCT_GrayRoughPlastic);
		AddSphere(Vector3{ 0.f, 3.f, 0.f }, .75f, matCT_GrayMediumPlastic);
		AddSphere(Vector3{ 1.75f, 3.f, 0.f }, .75f, matCT_GraySmoothPlastic);

		//CW Winding Order!
		const Triangle baseTriangle = { Vector3(-.75f, 1.5f, 0.f), Vector3(.75f, 0.f, 0.f), Vector3(-.75f, 0.f, 0.f) };

		m_Meshes.resize(3);

		m_Meshes[0] = AddTriangleMesh(TriangleCullMode::BackFaceCulling, matLambert_White);
		m_Meshes[0]->AppendTriangle(baseTriangle);
		m_Meshes[0]->Translate({ -1.75f,4.5f,0.f });
		m_Meshes[0]->UpdateAABB();
		m_Meshes[0]->UpdateTransforms();

		m_Meshes[1] = AddTriangleMesh(TriangleCullMode::FrontFaceCulling, matLambert_White);
		m_Meshes[1]->AppendTriangle(baseTriangle, true);
		m_Meshes[1]->Translate({ 0.f,4.5f,0.f });
		m_Meshes[1]->UpdateAABB();
		m_Meshes[1]->UpdateTransforms();

		m_Meshes[2] = AddTriangleMesh(TriangleCullMode::NoCulling, matLambert_White);
		m_Meshes[2]->AppendTriangle(baseTriangle, true);
		m_Meshes[2]->Translate({ 1.75f,4.5f,0.f });
		m_Meshes[2]->UpdateAABB();
		m_Meshes[2]->UpdateTransforms();

		AddPointLight(Vector3{ 0.f, 5.f, 5.f }, 50.f, ColorRGB{ 1.f, .61f, .45f }); //Backlight
		AddPointLight(Vector3{ -2.5f, 5.f, -5.f }, 70.f, ColorRGB{ 1.f, .8f, .45f }); //Front Light Left
		AddPointLight(Vector3{ 2.5f, 2.5f, -5.f }, 50.f, ColorRGB{ .34f, .47f, .68f });
	}

	void Scene_W4_ReferenceScene::Update(Timer* pTimer)
	{
		Scene::Update(pTimer);

		const float angle = (cosf(pTimer->GetTotal()) + 1.f) * 0.5f * PI_2;
		for (TriangleMesh& mesh : m_TriangleMeshGeometries)
		{
			mesh.RotateY(angle);
			mesh.UpdateTransforms();
		}
	}

	void Scene_W4_BunnyScene::Initialize()
	{
		sceneName = "Bunny Scene W4";
		m_Camera.origin = { 0.f, 3.f, -10.f };
		m_Camera.fovAngle = 45;
		m_Camera.fovValue = tanf((m_Camera.fovAngle * TO_RADIANS) * 0.5f);

		//Materials
		const unsigned char matLambert_GrayBlue = AddMaterial(new Material_Lambert({ .49f, 0.57f, 0.57f }, 1.f));
		const unsigned char matLambert_White = AddMaterial(new Material_Lambert(colors::White, 0.6f));

		//Planes
		AddPlane(Vector3{ 0.f, 0.f, 10.f }, Vector3{ 0.f, 0.f, -1.f }, matLambert_GrayBlue); //BACK
		AddPlane(Vector3{ 0.f, 0.f, 0.f }, Vector3{ 0.f, 1.f, 0.f }, matLambert_GrayBlue); //BOTTOM
		AddPlane(Vector3{ 0.f, 10.f, 0.f }, Vector3{ 0.f, -1.f, 0.f }, matLambert_GrayBlue); //TOP
		AddPlane(Vector3{ 5.f, 0.f, 0.f }, Vector3{ -1.f, 0.f, 0.f }, matLambert_GrayBlue); //RIGHT
		AddPlane(Vector3{ -5.f, 0.f, 0.f }, Vector3{ 1.f, 0.f, 0.f }, matLambert_GrayBlue); //LEFT

		////BUNNY OBJ
		////===
		m_pBunnyMesh = AddTriangleMesh(TriangleCullMode::BackFaceCulling, matLambert_White);
		Utils::ParseOBJ(
			"Resources/lowpoly_bunny.obj",
			m_pBunnyMesh->positions,
			m_pBunnyMesh->normals,
			m_pBunnyMesh->indices);

		m_pBunnyMesh->Scale({ 2.f, 2.f, 2.f });
		m_pBunnyMesh->Translate({ 0.f, 0.f, 0.f });
		m_pBunnyMesh->RotateY(PI);

		//No need to Calculate the normals, these are calculated inside the ParseOBJ function
		m_pBunnyMesh->UpdateAABB();
		m_pBunnyMesh->UpdateTransforms();

		//Light
		AddPointLight(Vector3{ 0.f, 5.f, 5.f }, 50.f, ColorRGB{ 1.f, .61f, .45f }); //Backlight
		AddPointLight(Vector3{ -2.5f, 5.f, -5.f }, 70.f, ColorRGB{ 1.f, .8f, .45f }); //Front Light Left
		AddPointLight(Vector3{ 2.5f, 2.5f, -5.f }, 50.f, ColorRGB{ .34f, .47f, .68f });
	}

	void Scene_W4_BunnyScene::Update(Timer* pTimer)
	{
		Scene::Update(pTimer);

		const float angle{ (cosf(pTimer->GetTotal()) + 1.f) * 0.5f * PI_2 };
		for (TriangleMesh& mesh : m_TriangleMeshGeometries)
		{
			mesh.RotateY(angle);
			mesh.UpdateTransforms();
		}
	}

	void Scene_W4_Extra::Initialize()
	{
		sceneName = "Bunny Scene W4";
		m_Camera.origin = { 0.f, 3.f, -10.f };
		m_Camera.fovAngle = 45;
		m_Camera.fovValue = tanf((m_Camera.fovAngle * TO_RADIANS) * 0.5f);

		//Materials
		const unsigned char matLambert_GrayBlue = AddMaterial(new Material_Lambert({ .49f, 0.57f, 0.57f }, 1.f));
		const unsigned char matCT_GreenMediumMetal = AddMaterial(new Material_CookTorrence({ .952f, .960f, .905f }, 1.f, 1.f));
		const unsigned char matCT_GrayMediumMetal = AddMaterial(new Material_CookTorrence({ .972f, .960f, .915f }, 1.f, .6f));

		////TRUCK OBJ
		////===
		m_Meshes.resize(2);

		m_Meshes[0] = AddTriangleMesh(TriangleCullMode::NoCulling, matCT_GreenMediumMetal);
		Utils::ParseOBJ(
			"Resources/truck.obj",
			m_Meshes[0]->positions,
			m_Meshes[0]->normals,
			m_Meshes[0]->indices);

		m_Meshes[0]->Scale({ 0.15f, 0.15f, 0.15f });
		m_Meshes[0]->RotateY((PI_DIV_2 * 0.5f) * 3.f + PI);
		m_Meshes[0]->Translate({ -1.f, 1.5f, -35.f });
		m_Meshes[0]->UpdateAABB();
		m_Meshes[0]->UpdateTransforms();

		//////BUNNY OBJ
		//////===

		m_Meshes[1] = AddTriangleMesh(TriangleCullMode::BackFaceCulling, matCT_GrayMediumMetal);
		Utils::ParseOBJ(
			"Resources/lowpoly_bunny.obj",
			m_Meshes[1]->positions,
			m_Meshes[1]->normals,
			m_Meshes[1]->indices);

		//No need to Calculate the normals, these are calculated inside the ParseOBJ function
		m_Meshes[1]->Scale({ 2.f, 2.f, 2.f });
		m_Meshes[1]->Translate({ 0.f, 0.f, 0.f });
		m_Meshes[1]->UpdateAABB();
		m_Meshes[1]->UpdateTransforms();

		// Planes
		AddPlane(Vector3{ 0.f, 0.f, 10.f }, Vector3{ 0.f, 0.f, -1.f }, matLambert_GrayBlue); //BACK
		AddPlane(Vector3{ 0.f, 0.f, 0.f }, Vector3{ 0.f, 1.f, 0.f }, matLambert_GrayBlue); //BOTTOM
		AddPlane(Vector3{ 0.f, 10.f, 0.f }, Vector3{ 0.f, -1.f, 0.f }, matLambert_GrayBlue); //TOP
		AddPlane(Vector3{ 5.f, 0.f, 0.f }, Vector3{ -1.f, 0.f, 0.f }, matLambert_GrayBlue); //RIGHT
		AddPlane(Vector3{ -5.f, 0.f, 0.f }, Vector3{ 1.f, 0.f, 0.f }, matLambert_GrayBlue); //LEFT

		//Light
		AddPointLight(Vector3{ 0.f, 5.f, 5.f }, 50.f, ColorRGB{ 1.f, .61f, .45f });
		AddPointLight(Vector3{ -2.5f, 5.f, -5.f }, 70.f, ColorRGB{ 1.f, .8f, .45f });
		AddPointLight(Vector3{ 2.5f, 2.5f, -5.f }, 50.f, ColorRGB{ .34f, .47f, .68f });

		AddPointLight(Vector3{ 0.f, 5.f, -40.f }, 50.f, ColorRGB{ 1.f, .61f, .45f });
		AddPointLight(Vector3{ -2.5f, 5.f, -30.f }, 70.f, ColorRGB{ 1.f, .8f, .45f });
		AddPointLight(Vector3{ 2.5f, 2.5f, -30.f }, 50.f, ColorRGB{ .34f, .47f, .68f });

		std::cout << "-------------------------------------------------------------------------------------------------\n";
		std::cout << "Scene discription:\n\n";
		std::cout << "\t\t - Turn around for epic mesh!\n";
		std::cout << "\t\t - 6 Lights in scene so best to put shadows off! (thats why default off)\n";
		std::cout << "\t\t - Enjoy fasinating bunny material!\n";
		std::cout << "\t\t - dFPS? -> press 'F' Key\n";
		std::cout << "-------------------------------------------------------------------------------------------------\n";
	}

	void Scene_W4_Extra::Update(Timer* pTimer)
	{
		Scene::Update(pTimer);

		m_Meshes[1]->RotateY(PI_DIV_2 * pTimer->GetTotal());
		m_Meshes[1]->UpdateTransforms();
	}
}