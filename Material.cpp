#include "Material.h"

namespace GfxEngine3D
{
	Material::Material(const std::shared_ptr<Texture>& diffuse, const glm::vec3& specular, float shininess)
		: m_diffuse(diffuse), m_specular(specular), m_shininess(shininess)
	{
	}
}