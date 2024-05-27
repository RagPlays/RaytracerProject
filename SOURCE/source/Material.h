#pragma once
#include "Math.h"
#include "DataTypes.h"
#include "BRDFs.h"

namespace dae
{
#pragma region Material BASE
	class Material
	{
	public:
		Material() = default;
		virtual ~Material() = default;

		Material(const Material&) = delete;
		Material(Material&&) noexcept = delete;
		Material& operator=(const Material&) = delete;
		Material& operator=(Material&&) noexcept = delete;

		/**
		 * \brief Function used to calculate the correct color for the specific material and its parameters
		 * \param hitRecord current hitrecord
		 * \param l light direction
		 * \param v view direction
		 * \return color
		 */

		virtual const ColorRGB Shade(const HitRecord& hitRecord = {}, const Vector3& l = {}, const Vector3& v = {}) = 0;
	};
#pragma endregion

#pragma region Material SOLID COLOR
	//SOLID COLOR
	//===========
	class Material_SolidColor final : public Material
	{
	public:
		Material_SolidColor(const ColorRGB& color)
			: m_Color{ color }
		{
		}

		const ColorRGB Shade(const HitRecord& hitRecord, const Vector3& l, const Vector3& v) override
		{
			return m_Color;
		}

	private:
		ColorRGB m_Color;
	};
#pragma endregion

#pragma region Material LAMBERT
	//LAMBERT
	//=======
	class Material_Lambert final : public Material
	{
	public:
		Material_Lambert(const ColorRGB& diffuseColor, const float diffuseReflectance) 
			: m_DiffuseColor{ diffuseColor } /*cd*/, m_DiffuseReflectance{ diffuseReflectance }
		{
		}

		const ColorRGB Shade(const HitRecord& hitRecord, const Vector3& l, const Vector3& v) override
		{
			return BRDF::Lambert(m_DiffuseReflectance, m_DiffuseColor);
		}

	private:
		ColorRGB m_DiffuseColor;
		float m_DiffuseReflectance; //kd
	};
#pragma endregion

#pragma region Material LAMBERT PHONG
	//LAMBERT-PHONG
	//=============
	class Material_LambertPhong final : public Material
	{
	public:
		Material_LambertPhong(const ColorRGB& diffuseColor, const float kd, const float ks, const float phongExponent)
			: m_DiffuseColor{ diffuseColor }, m_DiffuseReflectance{ kd }, m_SpecularReflectance{ ks },
			  m_PhongExponent{ phongExponent }
		{
		}

		const ColorRGB Shade(const HitRecord& hitRecord, const Vector3& l, const Vector3& v) override
		{
			return 
			{ 
				BRDF::Lambert(m_DiffuseReflectance, m_DiffuseColor) 
				+ 
				BRDF::Phong(m_SpecularReflectance, m_PhongExponent, l, v, hitRecord.normal)
			};
		}

	private:
		ColorRGB m_DiffuseColor;
		float m_DiffuseReflectance; //kd
		float m_SpecularReflectance; //ks
		float m_PhongExponent; //Phong Exponent
	};
#pragma endregion

#pragma region Material COOK TORRENCE
	//COOK TORRENCE
	class Material_CookTorrence final : public Material
	{
	public:
		Material_CookTorrence(const ColorRGB& albedo, const float metalness, const float roughness)
			: m_Albedo{ albedo }, m_Metalness{ metalness }, m_Roughness{ roughness }
		{
		}

		const ColorRGB Shade(const HitRecord& hitRecord, const Vector3& l, const Vector3& v) override
		{
			if (m_Roughness < FLT_EPSILON) return{};

			const ColorRGB	f0 = m_Metalness ? m_Albedo : ColorRGB{ 0.04f, 0.04f, 0.04f };
			const Vector3	h{ (v + l).Normalized() };

			const ColorRGB	F{ BRDF::FresnelFunction_Schlick(h, v, f0) };
			const float		D{ BRDF::NormalDistribution_GGX(hitRecord.normal, h, m_Roughness) };
			const float		G{ BRDF::GeometryFunction_Smith(hitRecord.normal, v, l, m_Roughness) };

			ColorRGB lambert{};
			if (!m_Metalness)
			{
				ColorRGB kd{ 1.f - F };
				lambert = BRDF::Lambert(kd, m_Albedo);
			}

			if (FloatIsZero(F.r) || FloatIsZero(D) || FloatIsZero(G)) return lambert;

			const ColorRGB	DFG{ D * F * G  };
			const float vnln4{ 4.f * Vector3::Dot(v, hitRecord.normal) * Vector3::Dot(l, hitRecord.normal) };
			const ColorRGB	specular{ DFG / vnln4 };

			return { lambert + specular };
		}

	private:
		ColorRGB m_Albedo; // material specific
		float m_Metalness;
		float m_Roughness; // [1.0 > 0.0] >> [ROUGH > SMOOTH]
	};
#pragma endregion
}
