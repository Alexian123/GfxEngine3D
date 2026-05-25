#pragma once

#include <glm/vec3.hpp>

namespace GfxEngine3D
{
	class Material
	{
	public:
		Material();
		Material(
			const glm::vec3& ambient,
			const glm::vec3& diffuse,
			const glm::vec3& specular,
			float shininess
		);

		~Material() = default;

		Material(const Material& other);
		Material& operator=(const Material& other);

		const glm::vec3& GetAmbient() const { return m_ambient; }
		const glm::vec3& GetDiffuse() const { return m_diffuse; }
		const glm::vec3& GetSpecular() const { return m_specular; }
		float GetShininess() const { return m_shininess; }

		void SetAmbient(const glm::vec3& ambient) { m_ambient = ambient; }
		void SetDiffuse(const glm::vec3& diffuse) { m_diffuse = diffuse; }
		void SetSpecular(const glm::vec3& specular) { m_specular = specular; }
		void SetShininess(float shininess) { m_shininess = shininess; }

	private:
		glm::vec3 m_ambient;
		glm::vec3 m_diffuse;
		glm::vec3 m_specular;
		float m_shininess;
	};
}
