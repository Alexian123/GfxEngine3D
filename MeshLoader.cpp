#include "MeshLoader.h"
#include "CubeMeshData.h"

#include <vector>

namespace GfxEngine3D
{
	MeshLoader::~MeshLoader()
	{
		m_meshCache.clear();
	}

	MeshLoader& MeshLoader::GetInstance()
	{
		static MeshLoader instance;
		return instance;
	}

	std::shared_ptr<Mesh> MeshLoader::LoadCube(const std::string& name, unsigned int attributes)
	{
		if (name.empty())
			return nullptr;
		auto cachedMesh = GetMeshFromCache(name);
		if (cachedMesh)
			return cachedMesh;

		std::vector<VertexData> vertices;
		for (size_t i = 0; i < CubeMeshData::VertexCount; ++i) {
			VertexData vertex{};
			vertex.position[0] = CubeMeshData::Positions[i * 3 + 0];
			vertex.position[1] = CubeMeshData::Positions[i * 3 + 1];
			vertex.position[2] = CubeMeshData::Positions[i * 3 + 2];
			if (attributes & Mesh::Color) {
				vertex.color[0] = CubeMeshData::Colors[i * 3 + 0];
				vertex.color[1] = CubeMeshData::Colors[i * 3 + 1];
				vertex.color[2] = CubeMeshData::Colors[i * 3 + 2];
			}
			if (attributes & Mesh::TexCoord) {
				vertex.texCoord[0] = CubeMeshData::TexCoords[i * 2 + 0];
				vertex.texCoord[1] = CubeMeshData::TexCoords[i * 2 + 1];
			}
			if (attributes & Mesh::Normal) {
				vertex.normal[0] = CubeMeshData::Normals[i * 3 + 0];
				vertex.normal[1] = CubeMeshData::Normals[i * 3 + 1];
				vertex.normal[2] = CubeMeshData::Normals[i * 3 + 2];
			}
			vertices.push_back(vertex);
		}

		std::shared_ptr<Mesh> cubeMesh = std::make_shared<Mesh>(vertices, attributes);
		m_meshCache[name] = cubeMesh;
		return cubeMesh;
	}

	MeshLoader::MeshLoader()
	{
		m_meshCache.reserve(10);	// reserve some space for caching meshes
	}

	std::shared_ptr<Mesh> MeshLoader::GetMeshFromCache(const std::string& name)
	{
		auto it = m_meshCache.find(name);
		if (it != m_meshCache.end())
			return it->second;
		return nullptr;
	}
}