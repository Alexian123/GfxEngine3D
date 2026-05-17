#include "GLBuffer.h"

#include <glad/glad.h>

GLBuffer::GLBuffer(BufferType type)
{
	switch (type) {
	case ArrayBuffer:
		m_target = GL_ARRAY_BUFFER;
		break;

	case ElementArrayBuffer:
		m_target = GL_ELEMENT_ARRAY_BUFFER;
		break;

	default:
		m_target = GL_ARRAY_BUFFER;
	}
	glGenBuffers(1, &m_id);
}

GLBuffer::~GLBuffer()
{
	glDeleteBuffers(1, &m_id);
	Unbind();
}

void GLBuffer::Bind() const
{
	glBindBuffer(m_target, m_id);
}

void GLBuffer::Unbind() const
{
	glBindBuffer(m_target, 0);
}

void GLBuffer::WriteData(const void* data, size_t size, unsigned int usage) const
{
	Bind();
	glBufferData(m_target, size, data, usage);
}
