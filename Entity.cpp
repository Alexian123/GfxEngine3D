#include "Entity.h"

#include <glm/gtc/matrix_transform.hpp>

namespace GfxEngine3D
{
	Entity::Entity(
		const std::shared_ptr<Mesh>& mesh, 
		const std::shared_ptr<Texture>& texture, 
		const std::shared_ptr<Material>& material
	) : m_id(GetNextID()), m_mesh(mesh), m_texture(texture), m_material(material)
	{
		UpdateMatricesIfNeeded();
	}

	const glm::mat4& Entity::GetModelMatrix()
	{
		UpdateMatricesIfNeeded();
		return m_modelMatrix;
	}

	const glm::mat4& Entity::GetNormalMatrix()
	{
		UpdateMatricesIfNeeded();
		return m_normalMatrix;
	}

	void Entity::SetPosition(const glm::vec3& position)
	{
		m_position = position;
		m_matricesDirty = true;
	}

	void Entity::SetRotation(const glm::vec3& rotation)
	{
		m_rotation = rotation;
		m_matricesDirty = true;
	}

	void Entity::SetScale(const glm::vec3& scale)
	{
		m_scale = scale;
		m_matricesDirty = true;
	}

	unsigned int Entity::GetNextID()
	{
		static unsigned int id = 0;
		return id++;
	}

	void Entity::UpdateMatricesIfNeeded()
	{
		if (!m_matricesDirty)
			return;
		m_modelMatrix = glm::translate(glm::mat4(1.0f), m_position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.y), glm::vec3(0, 1, 0))		// YXZ rotation
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.x), glm::vec3(1, 0, 0))
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.z), glm::vec3(0, 0, 1))
			* glm::scale(glm::mat4(1.0f), m_scale);
		m_normalMatrix = glm::mat4(glm::mat3(glm::transpose(glm::inverse(m_modelMatrix))));
		m_matricesDirty = false;
	}
}