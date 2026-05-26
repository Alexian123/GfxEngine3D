#pragma once

#include <memory>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "Mesh.h"
#include "Texture.h"
#include "Material.h"

namespace GfxEngine3D
{
	class Entity
	{
	public:
		Entity(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material);

		~Entity() = default;

		Entity(const Entity&) = delete;
		Entity& operator=(const Entity&) = delete;

		const std::shared_ptr<Mesh>& GetMesh() const { return m_mesh; }
		const std::shared_ptr<Material>& GetMaterial() const { return m_material; }
		const glm::vec3& GetPosition() const { return m_position; }
		const glm::vec3& GetRotation() const { return m_rotation; }
		const glm::vec3& GetScale() const { return m_scale; };

		const glm::mat4& GetModelMatrix();
		const glm::mat4& GetNormalMatrix();

		void SetMesh(const std::shared_ptr<Mesh>& mesh) { m_mesh = mesh; }
		void SetMaterial(const std::shared_ptr<Material>& material) { m_material = material; }

		void SetPosition(const glm::vec3& position);
		void SetRotation(const glm::vec3& rotation);
		void SetScale(const glm::vec3& scale);
		void SetScale(float scale);

	protected:
		Entity();

	private:
		unsigned int m_id;
		std::shared_ptr<Mesh> m_mesh;
		std::shared_ptr<Material> m_material;
		glm::vec3 m_position = glm::vec3(0.0f); 
		glm::vec3 m_rotation = glm::vec3(0.0f);
		glm::vec3 m_scale = glm::vec3(1.0f);
		glm::mat4 m_modelMatrix;
		glm::mat4 m_normalMatrix;
		bool m_matricesDirty = true;

		static unsigned int GetNextID();
		void UpdateMatricesIfNeeded();
	};
}
