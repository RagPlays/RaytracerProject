//External includes
#include <iostream>
#include <execution>
#include "SDL.h"
#include "SDL_surface.h"

//Project includes
#include "Renderer.h"
#include "Math.h"
#include "Matrix.h"
#include "Material.h"
#include "Scene.h"
#include "Utils.h"

#define PARALLEL_EXECUTION

using namespace dae;

Renderer::Renderer(SDL_Window* pWindow, const int width, const int height)
	: m_pWindow{ pWindow },
	m_pBuffer{ SDL_GetWindowSurface(pWindow) },
	m_Width{ width },
	m_Height{ height },
	m_NrOfPixels{ static_cast<uint32_t>(width * height) },
	m_AspectRatio{ float(m_Width) / m_Height }
{
	//Initialize
	m_pBufferPixels = static_cast<uint32_t*>(m_pBuffer->pixels);

	m_PixelIndices.resize(m_NrOfPixels);
	for (uint32_t pixelIdx{}; pixelIdx < m_NrOfPixels; ++pixelIdx)
	{
		m_PixelIndices[pixelIdx] = pixelIdx;
	}
}

void Renderer::Render(Scene* pScene) const
{
	// ................................................................................................................;
	Camera& camera{ pScene->GetCamera() };
	const std::vector< dae::Material* >& materials{ pScene->GetMaterials() };
	const std::vector< dae::Light >& lights{ pScene->GetLights() };

	const Matrix cameraToWorld{ camera.CalculateCameraToWorld() };

#ifdef PARALLEL_EXECUTION
	// Parallel logic //
	std::for_each(std::execution::par, m_PixelIndices.begin(), m_PixelIndices.end(), [&](int i)
	{
		RenderPixel(pScene, materials, lights, i, camera.fovValue, cameraToWorld, camera.origin);
	});

#else 
	// Synchornous logic (no threading) //
	for (uint32_t pixelIdx{}; pixelIdx < m_NrOfPixels; ++pixelIdx)
	{
		RenderPixel(pScene, materials, lights, pixelIdx, camera.fovValue, cameraToWorld, camera.origin);
	}

#endif
	// ................................................................................................................;

	//@END
	//Update SDL Surface
	SDL_UpdateWindowSurface(m_pWindow);
}

void dae::Renderer::RenderPixel(
	Scene* pScene, 
	const std::vector< dae::Material* >& materials,
	const std::vector< dae::Light >& lights,
	const uint32_t pixelIndex, 
	const float fov,
	const Matrix& cameraToWorld, 
	const Vector3& cameraOrigin) const
{
	constexpr int colorCorrector{ 255 };
	constexpr float offset{ 0.00001f };

	const uint32_t px{ pixelIndex % m_Width };
	const uint32_t py{ pixelIndex / m_Width };

	const float rx{ px + 0.5f };
	const float ry{ py + 0.5f };

	const float pxC{ ((rx / m_Width * 2.f) - 1.f) * m_AspectRatio * fov };
	const float pyC{ (1.f - ry / m_Height * 2.f) * fov };

	ColorRGB finalColor;

	HitRecord closestHit;

	Vector3 rayDirection{ -cameraToWorld.TransformVector(pxC, pyC, 1.f).Normalized() };

	Ray vieuwRay{ cameraOrigin, -rayDirection };

	if (pScene->GetClosestHit(vieuwRay, closestHit))
	{
		for (const Light& light : lights)
		{
			Vector3 lightDirection{ LightUtils::GetDirectionToLight(light, closestHit.origin) };

			const float maxLightRay{ lightDirection.Normalize() };

			Ray lightRay{ closestHit.origin + (closestHit.normal * offset), lightDirection };

			lightRay.max = maxLightRay;

			if (!(m_ShadowEnabled && pScene->DoesHit(lightRay)))
			{
				const float observedArea{ Vector3::Dot(closestHit.normal, lightDirection) };
				if (!(observedArea < 0.f))
				{
					const dae::ColorRGB radiance = LightUtils::GetRadiance(light, closestHit.origin);
					const dae::ColorRGB BRDFColor{ materials[closestHit.materialIndex]->Shade(closestHit, lightDirection, rayDirection) };

					switch (m_CurrentLightMode)
					{
					case dae::Renderer::LightingMode::ObserverdArea:
						finalColor += observedArea;
						break;

					case dae::Renderer::LightingMode::Radiance:
						finalColor += radiance;
						break;

					case dae::Renderer::LightingMode::BRDF:
						finalColor += BRDFColor;
						break;

					case dae::Renderer::LightingMode::Combined:
						finalColor += radiance * BRDFColor * observedArea;
						break;
					}
				}
			}
		}
	}

	finalColor.MaxToOne();
	m_pBufferPixels[px + (py * m_Width)] = SDL_MapRGB(m_pBuffer->format,
		static_cast<uint8_t>(finalColor.r * colorCorrector),
		static_cast<uint8_t>(finalColor.g * colorCorrector),
		static_cast<uint8_t>(finalColor.b * colorCorrector));
}

bool Renderer::SaveBufferToImage() const
{
	return SDL_SaveBMP(m_pBuffer, "RayTracing_Buffer.bmp");
}

void Renderer::CycleLightingMode()
{
	switch (m_CurrentLightMode)
	{
	case dae::Renderer::LightingMode::ObserverdArea:
		m_CurrentLightMode = LightingMode::Radiance;
		std::cout << "LIGHTINGMODE: RADIANCE\n";
		return;

	case dae::Renderer::LightingMode::Radiance:
		m_CurrentLightMode = LightingMode::BRDF;
		std::cout << "LIGHTINGMODE: BRDF\n";
		return;

	case dae::Renderer::LightingMode::BRDF:
		m_CurrentLightMode = LightingMode::Combined;
		std::cout << "LIGHTINGMODE: COMBINED\n";
		return;

	case dae::Renderer::LightingMode::Combined:
		m_CurrentLightMode = LightingMode::ObserverdArea;
		std::cout << "LIGHTINGMODE: OBSERVERD AREA\n";
		return;
	}
}

void Renderer::ToggleShadows()
{
	m_ShadowEnabled = !m_ShadowEnabled;

	if (m_ShadowEnabled)
	{
		std::cout << "Shadow ON\n";
		return;
	}
	std::cout << "Shadow OFF\n";
}