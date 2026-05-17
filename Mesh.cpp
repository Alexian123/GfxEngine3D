#include "Mesh.h"

#include <glad/glad.h>

Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, bool hasColors)
	: m_indexCount(indices.size())
{
	// create VAO
	glGenVertexArrays(1, &m_VAO);

	Bind();

	// create VBO
	m_VBO.WriteData(vertices.data(), vertices.size() * sizeof(float), GL_STATIC_DRAW);

	int stride = hasColors ? 6 * sizeof(float) : 3 * sizeof(float);
	m_vertexCount = vertices.size() / (stride / sizeof(float));	// size / floats per vertex

	// vertex position attribute
	setVertexAttribute(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

	// vertex color attribute
	if (hasColors) {
		setVertexAttribute(1, 3, GL_FLOAT, GL_FALSE, stride, 3 * sizeof(float));
	}

	// vertex normal attribute

	// create EBO
	m_EBO.WriteData(indices.data(), indices.size() * sizeof(unsigned int), GL_STATIC_DRAW);

	Unbind();
}

Mesh::Mesh(const std::vector<float>& vertices, bool hasColors)
{
	// create VAO
	glGenVertexArrays(1, &m_VAO);

	Bind();

	// create VBO
	m_VBO.WriteData(vertices.data(), vertices.size() * sizeof(float), GL_STATIC_DRAW);

	int stride = hasColors ? 6 * sizeof(float) : 3 * sizeof(float);
	m_vertexCount = vertices.size() / (stride / sizeof(float));

	// vertex position attribute
	setVertexAttribute(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

	// vertex color attribute
	if (hasColors) {
		setVertexAttribute(1, 3, GL_FLOAT, GL_FALSE, stride, 3 * sizeof(float));
	}

	Unbind();
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_VAO);
}

void Mesh::Bind() const
{
	glBindVertexArray(m_VAO);
}

void Mesh::Unbind() const
{
	glBindVertexArray(0);
}

void Mesh::Draw() const
{
	if (m_indexCount > 0) {
		glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
	}
}

void Mesh::setVertexAttribute(
	unsigned int index,
	unsigned int size,
	unsigned int type,
	bool normalized,
	unsigned int stride,
	unsigned int offset
) const
{
	glVertexAttribPointer(index, size, type, normalized, stride, 
		reinterpret_cast<void*>(static_cast<uintptr_t>(offset)));
	glEnableVertexAttribArray(index);
}