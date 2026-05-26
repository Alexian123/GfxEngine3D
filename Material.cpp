#include "Material.h"

namespace GfxEngine3D
{
	Material::Material(
		const std::shared_ptr<Texture>& diffuse, 
		const std::shared_ptr<Texture>& specular,
		float shininess,
		const std::shared_ptr<Texture>& emission
	) : m_diffuse(diffuse), m_specular(specular), m_shininess(shininess), m_emission(emission)
	{
	}
}