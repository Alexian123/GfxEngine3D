#pragma once

#include <vector>

#include "GLBuffer.h"

class Mesh
{
public:
	enum VertexDataComponent
	{
		// in order of components in vertex data
		Color = 0x1,
		TexCoord = 0x2,
		Normal = 0x4
	};

	Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, unsigned int components = 0);
	Mesh(const std::vector<float>& vertices, unsigned int components = 0);
	~Mesh();

	void Bind() const;
	void Unbind() const;
	void Draw() const;

	Mesh() = delete;
	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

private:
	size_t m_vertexCount = 0;
	size_t m_indexCount = 0;
	unsigned int m_VAO = 0;
	GLBuffer m_VBO{ GLBuffer::ArrayBuffer };
	GLBuffer m_EBO{ GLBuffer::ElementArrayBuffer };

	void setVertexAttribute(
		unsigned int index,
		unsigned int size,
		unsigned int type,
		bool normalized,
		unsigned int stride,
		unsigned int offset
	) const;
};