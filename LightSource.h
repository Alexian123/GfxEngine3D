#pragma once

#include <memory>

#include <glm/vec3.hpp>

#include "Entity.h"

namespace GfxEngine3D
{

	class LightSource : public Entity
	{
	public:
		LightSource(
			const glm::vec3& position,
			const glm::vec3& ambient,
			const glm::vec3& diffuse,
			const glm::vec3& specular
		);

		LightSource() = delete;
		LightSource(const LightSource&) = delete;
		LightSource& operator=(const LightSource&) = delete;
		~LightSource() = default;

		const glm::vec3& GetAmbient() const { return m_ambient; }
		const glm::vec3& GetDiffuse() const { return m_diffuse; }
		const glm::vec3& GetSpecular() const { return m_specular; }

		void SetAmbient(const glm::vec3& ambient) { m_ambient = ambient; }
		void SetDiffuse(const glm::vec3& diffuse) { m_diffuse = diffuse; }
		void SetSpecular(const glm::vec3& specular) { m_specular = specular; }

	private:
		glm::vec3 m_ambient;
		glm::vec3 m_diffuse;
		glm::vec3 m_specular;
	};

}