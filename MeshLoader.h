#pragma once

#include "Mesh.h"

#include <unordered_map>
#include <string>
#include <memory>

namespace GfxEngine3D
{
	class MeshLoader
	{
	public:
		~MeshLoader();

		static MeshLoader& GetInstance();

		std::shared_ptr<Mesh> LoadCube(const std::string& name, unsigned int attributes = Mesh::Position);

		MeshLoader(const MeshLoader&) = delete;
		MeshLoader& operator=(const MeshLoader&) = delete;

	protected:
		MeshLoader();

	private:
		std::unordered_map<std::string, std::shared_ptr<Mesh>> m_meshCache;

		std::shared_ptr<Mesh> GetMeshFromCache(const std::string& name);
	};
}