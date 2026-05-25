#pragma once

#include <vector>

#include "GLBuffer.h"

namespace GfxEngine3D
{
	struct VertexData
	{
		float position[3];
		float color[3];
		float texCoord[2];
		float normal[3];
	};

	class Mesh
	{
	public:
		enum VertexAttribute
		{
			// in order of attributes in vertex data
			Position = 0x0,
			Color = 0x1,
			TexCoord = 0x2,
			Normal = 0x4
		};

		Mesh(const std::vector<VertexData>& vertices, const std::vector<unsigned int>& indices, unsigned int attributes = Position);
		Mesh(const std::vector<VertexData>& vertices, unsigned int attributes = Position);
		~Mesh();

		void Bind() const;
		void Unbind() const;
		void Draw() const;

		int GetVertexCount() const { return m_vertexCount; }
		int GetIndexCount() const { return m_indexCount; }
		unsigned int GetAttributes() const { return m_attributes; }

		Mesh() = delete;
		Mesh(const Mesh&) = delete;
		Mesh& operator=(const Mesh&) = delete;

	private:
		int m_vertexCount = 0;
		int m_indexCount = 0;
		unsigned int m_VAO = 0;
		GLBuffer m_VBO{ GLBuffer::ArrayBuffer };
		GLBuffer m_EBO{ GLBuffer::ElementArrayBuffer };
		unsigned int m_attributes = Position;

		std::vector<float> BuildVertexBuffer(const std::vector<VertexData>& vertices, unsigned int attributes) const;

		unsigned int CalculateStride(unsigned int attributes) const;

		void setVertexAttribute(
			unsigned int index,
			unsigned int size,
			unsigned int type,
			bool normalized,
			unsigned int stride,
			unsigned int offset
		) const;

		void disableVertexAttibute(unsigned int index) const;
	};
}