#include "Material.h"

namespace GfxEngine3D
{
	Material::Material()
		: Material(glm::vec3(0.3f), glm::vec3(1.0f), glm::vec3(1.0f), 32.0f)
	{
	}

	Material::Material(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess)
		: m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_shininess(shininess)
	{
	}

	Material::Material(const Material& other)
		: m_ambient(other.m_ambient), m_diffuse(other.m_diffuse), 
		m_specular(other.m_specular), m_shininess(other.m_shininess)
	{
	}

	Material& Material::operator=(const Material& other)
	{
		m_ambient = other.m_ambient;
		m_diffuse = other.m_diffuse;
		m_specular = other.m_specular;
		m_shininess = other.m_shininess;
		return *this;
	}
}