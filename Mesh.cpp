#include "Mesh.h"

#include <glad/glad.h>

Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, unsigned int components)
	: m_indexCount(indices.size())
{
	// create VAO
	glGenVertexArrays(1, &m_VAO);

	Bind();

	// create VBO
	m_VBO.WriteData(vertices.data(), vertices.size() * sizeof(float), GL_STATIC_DRAW);

	// calculate stride and vertex count
	int stride = 3; // position
	if (components & Color) {
		stride += 3;
	}
	if (components & TexCoord) {
		stride += 2;
	}
	if (components & Normal) {
		stride += 3;
	}
	stride *= sizeof(float);
	m_vertexCount = vertices.size() / (stride / sizeof(float));	// size / floats per vertex
	int offset = 0;

	// position attribute
	setVertexAttribute(0, 3, GL_FLOAT, GL_FALSE, stride, offset);
	offset += 3 * sizeof(float);

	// color attribute
	if (components & Color) {
		setVertexAttribute(1, 3, GL_FLOAT, GL_FALSE, stride, offset);
		offset += 3 * sizeof(float);
	}
	else {
		disableVertexAttibute(1);
	}

	// tex coord attribute
	if (components & TexCoord) {
		setVertexAttribute(2, 2, GL_FLOAT, GL_FALSE, stride, offset);
		offset += 2 * sizeof(float);
	}
	else {
		disableVertexAttibute(2);
	}

	// normal attribute
	if (components & Normal) {
		setVertexAttribute(3, 3, GL_FLOAT, GL_FALSE, stride, offset);
		offset += 3 * sizeof(float);
	}
	else {
		disableVertexAttibute(3);
	}

	// create EBO
	if (indices.size() > 0) {
		m_EBO.WriteData(indices.data(), indices.size() * sizeof(unsigned int), GL_STATIC_DRAW);
	}

	Unbind();
}

Mesh::Mesh(const std::vector<float>& vertices, unsigned int components)
	: Mesh(vertices, std::vector<unsigned int>(), components)	// empty indices vector
{
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
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, type, normalized, stride, 
		reinterpret_cast<void*>(static_cast<uintptr_t>(offset)));
}

void Mesh::disableVertexAttibute(unsigned int index) const
{
	glDisableVertexAttribArray(index);
}
