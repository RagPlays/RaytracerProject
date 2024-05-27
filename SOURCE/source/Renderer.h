#pragma once

struct SDL_Window;
struct SDL_Surface;

namespace dae
{
	class Scene;
	class Material;

	struct Matrix;
	struct Vector3;
	struct Light;

	class Renderer final
	{
	public:
		Renderer(SDL_Window* pWindow, const int width, const int height);
		~Renderer() = default;

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;

		void Render(Scene* pScene) const;

		void RenderPixel(
			Scene* pScene,
			const std::vector< dae::Material* >& materials,
			const std::vector< dae::Light >& lights,
			const uint32_t pixelIndex,
			const float fov,
			const Matrix& cameraToWorld,
			const Vector3& cameraOrigin) const;

		bool SaveBufferToImage() const;

		void CycleLightingMode();
		void ToggleShadows();

	private:

		// defaults //
		SDL_Window* m_pWindow{};

		SDL_Surface* m_pBuffer{};
		uint32_t* m_pBufferPixels{};

		const int m_Width;
		const int m_Height;
		const float m_AspectRatio;

		// Toggles //
		bool m_ShadowEnabled{ true };

		enum class LightingMode
		{
			ObserverdArea = 0,	// Lambert cosine law
			Radiance,			// Incident radiance
			BRDF,				// Scattering of the light
			Combined			// ObservedArea * Radiance * BRDF
		};
		LightingMode m_CurrentLightMode{ LightingMode::Combined };

		// multithreading
		std::vector<uint32_t> m_PixelIndices{};
		const uint32_t m_NrOfPixels;
		int m_ShadowFrame{};
	};
}
