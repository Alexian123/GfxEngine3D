#pragma once

#include <memory>

#include <glm/vec3.hpp>

#include "Texture.h"

namespace GfxEngine3D
{
	class Material
	{
	public:
		Material(
			const std::shared_ptr<Texture>& diffuse, 
			const std::shared_ptr<Texture>& specular, 
			float shininess,
			const std::shared_ptr<Texture>& emission = nullptr
		);

		~Material() = default;

		Material() = delete;
		Material(const Material& other) = delete;
		Material& operator=(const Material& other) = delete;

		const std::shared_ptr<Texture>& GetDiffuse() const { return m_diffuse; }
		const std::shared_ptr<Texture>& GetSpecular() const { return m_specular; }
		const std::shared_ptr<Texture>& GetEmission() const { return m_emission; }
		float GetShininess() const { return m_shininess; }

		void SetDiffuse(const std::shared_ptr<Texture>& diffuse) { m_diffuse = diffuse; }
		void SetSpecular(const std::shared_ptr<Texture>& specular) { m_specular = specular; }
		void SetEmission(const std::shared_ptr<Texture>& emission) { m_emission = emission; }
		void SetShininess(float shininess) { m_shininess = shininess; }

	private:
		std::shared_ptr<Texture> m_diffuse;
		std::shared_ptr<Texture> m_specular;
		std::shared_ptr<Texture> m_emission;
		float m_shininess;
	};
}
