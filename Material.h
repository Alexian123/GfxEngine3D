#pragma once

#include <memory>

#include <glm/vec3.hpp>

#include "Texture.h"

namespace GfxEngine3D
{
	class Material
	{
	public:
		Material(const std::shared_ptr<Texture>& diffuse, const glm::vec3& specular, float shininess);

		~Material() = default;

		Material() = delete;
		Material(const Material& other) = delete;
		Material& operator=(const Material& other) = delete;

		const std::shared_ptr<Texture>& GetDiffuse() const { return m_diffuse; }
		const glm::vec3& GetSpecular() const { return m_specular; }
		float GetShininess() const { return m_shininess; }

		void SetDiffuse(const std::shared_ptr<Texture>& diffuse) { m_diffuse = diffuse; }
		void SetSpecular(const glm::vec3& specular) { m_specular = specular; }
		void SetShininess(float shininess) { m_shininess = shininess; }

	private:
		std::shared_ptr<Texture> m_diffuse;
		glm::vec3 m_specular;
		float m_shininess;
	};
}
