#include "Mesh.h"

#include <glad/glad.h>

namespace GfxEngine3D
{
	Mesh::Mesh(const std::vector<VertexData>& vertices, const std::vector<unsigned int>& indices, unsigned int attributes)
		: m_indexCount(static_cast<int>(indices.size())), m_attributes(attributes)
	{
		// create VAO
		glGenVertexArrays(1, &m_VAO);

		Bind();

		// create VBO
		std::vector<float> vertexBuffer = BuildVertexBuffer(vertices, attributes);
		m_VBO.WriteData(vertexBuffer.data(), static_cast<int>(vertexBuffer.size() * sizeof(float)), GL_STATIC_DRAW);

		// calculate stride and vertex count
		unsigned int stride = CalculateStride(attributes);
		m_vertexCount = static_cast<int>(vertices.size());
		unsigned int offset = 0;

		// position attribute
		setVertexAttribute(0, 3, GL_FLOAT, GL_FALSE, stride, offset);
		offset += sizeof(VertexData::position);

		// tex coord attribute
		if (attributes & TexCoord) {
			setVertexAttribute(1, 2, GL_FLOAT, GL_FALSE, stride, offset);
			offset += sizeof(VertexData::texCoord);
		}
		else {
			disableVertexAttibute(2);
		}

		// normal attribute
		if (attributes & Normal) {
			setVertexAttribute(2, 3, GL_FLOAT, GL_FALSE, stride, offset);
			offset += sizeof(VertexData::normal);
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

	Mesh::Mesh(const std::vector<VertexData>& vertices, unsigned int attributes)
		: Mesh(vertices, std::vector<unsigned int>(), attributes)	// empty indices vector
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

	std::vector<float> Mesh::BuildVertexBuffer(const std::vector<VertexData>& vertices, unsigned int attributes) const
	{
		std::vector<float> vertexBuffer;
		for (const auto& vertex : vertices) {
			// position
			vertexBuffer.insert(vertexBuffer.end(), std::begin(vertex.position), std::end(vertex.position));
			// tex coord
			if (attributes & TexCoord) {
				vertexBuffer.insert(vertexBuffer.end(), std::begin(vertex.texCoord), std::end(vertex.texCoord));
			}
			// normal
			if (attributes & Normal) {
				vertexBuffer.insert(vertexBuffer.end(), std::begin(vertex.normal), std::end(vertex.normal));
			}
		}
		return vertexBuffer;
	}

	unsigned int Mesh::CalculateStride(unsigned int attributes) const
	{
		unsigned int stride = 3;
		if (attributes & TexCoord) stride += 2;
		if (attributes & Normal)   stride += 3;
		return stride * sizeof(float);
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
}