#pragma once
#include "Math.h"

namespace dae
{
	namespace BRDF
	{
		/**
		 * \param kd Diffuse Reflection Coefficient
		 * \param cd Diffuse Color
		 * \return Lambert Diffuse Color
		 */
		static const ColorRGB Lambert(const float kd, const ColorRGB& cd)
		{
			return { cd * kd * DIV_PI };
		}

		static const ColorRGB Lambert(const ColorRGB& kd, const ColorRGB& cd)
		{
			return { cd * kd * DIV_PI };
		}

		/**
		 * \brief todo
		 * \param ks Specular Reflection Coefficient
		 * \param exp Phong Exponent
		 * \param l Incoming (incident) Light Direction
		 * \param v View Direction
		 * \param n Normal of the Surface
		 * \return Phong Specular Color
		 */
		static const ColorRGB Phong(const float ks, const float exp, const Vector3& l, const Vector3& v, const Vector3& n)
		{
			const float cosA{ Vector3::Dot(Vector3::Reflect(l, v), v) };

			if (cosA < FLT_EPSILON) return {};

			return { ks * powf(cosA, exp) };
		}

		/**
		 * \brief BRDF Fresnel Function >> Schlick
		 * \param h Normalized Halfvector between View and Light directions
		 * \param v Normalized View direction
		 * \param f0 Base reflectivity of a surface based on IOR (Indices Of Refrection), this is different for Dielectrics (Non-Metal) and Conductors (Metal)
		 * \return
		 */
		static const ColorRGB FresnelFunction_Schlick(const Vector3& h, const Vector3& v, const ColorRGB& f0)
		{
			const float temp{ 1.f - Vector3::Dot(h, v) };
			const float tempSquared{ temp * temp };
			const float tempPow5{ temp * tempSquared * tempSquared };

			return { f0 + (1.f - f0) * tempPow5 };
		}

		/**
		 * \brief BRDF NormalDistribution >> Trowbridge-Reitz GGX (UE4 implemetation - squared(roughness))
		 * \param n Surface normal
		 * \param h Normalized half vector
		 * \param roughness Roughness of the material
		 * \return BRDF Normal Distribution Term using Trowbridge-Reitz GGX
		 */
		static const float NormalDistribution_GGX(const Vector3& n, const Vector3& h, const float roughness)
		{
			const float a{ roughness * roughness };
			const float aa{  a * a };

			const float dot{ Vector3::Dot(n, h) };

			if (dot < FLT_EPSILON) return {};

			const float divider{ PI * powf((dot * dot * (aa - 1.f) + 1.f), 2.f) };

			return (aa / divider);
		}

		/**
		 * \brief BRDF Geometry Function >> Schlick GGX (Direct Lighting + UE4 implementation - squared(roughness))
		 * \param n Normal of the surface
		 * \param v Normalized view direction
		 * \param roughness Roughness of the material
		 * \return BRDF Geometry Term using SchlickGGX
		 */
		static const float GeometryFunction_SchlickGGX(const Vector3& n, const Vector3& v, const float k)
		{
			//const float k{ ((a + 1) * (a + 1)) / 8 }; // direct
			//const float k{ powf(a, 2) / 2.f }; // indirect

			const float dot{ Vector3::Dot(n, v) };

			if (dot < FLT_EPSILON) return {};

			const float temp{ dot * (1 - k) + k };

			return { dot / temp };
		}

		/**
		 * \brief BRDF Geometry Function >> Smith (Direct Lighting)
		 * \param n Normal of the surface
		 * \param v Normalized view direction
		 * \param l Normalized light direction
		 * \param roughness Roughness of the material
		 * \return BRDF Geometry Term using Smith (> SchlickGGX(n,v,roughness) * SchlickGGX(n,l,roughness))
		 */
		static const float GeometryFunction_Smith(const Vector3& n, const Vector3& v, const Vector3& l, const float roughness)
		{
			const float k{ Square(Square(roughness) + 1) / 8.f };

			return { GeometryFunction_SchlickGGX(n, v, k) * GeometryFunction_SchlickGGX(n, l, k) };
		}

	}
}