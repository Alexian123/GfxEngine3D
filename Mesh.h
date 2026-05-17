#pragma once

#include <vector>

#include "GLBuffer.h"

class Mesh
{
public:
	Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, bool hasColors = false);
	Mesh(const std::vector<float>& vertices, bool hasColors = false);
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