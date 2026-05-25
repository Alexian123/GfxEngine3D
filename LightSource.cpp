#include "LightSource.h"

namespace GfxEngine3D
{
	LightSource::LightSource(
		const glm::vec3& position, 
		const glm::vec3& ambient, 
		const glm::vec3& diffuse, 
		const glm::vec3& specular
	) : Entity(), m_ambient(ambient), m_diffuse(diffuse), m_specular(specular)
	{
		SetPosition(position);
	}
}