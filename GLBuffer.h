#pragma once

#include <stddef.h>

class GLBuffer
{
public:
	enum BufferType
	{
		ArrayBuffer,
		ElementArrayBuffer
	};

	GLBuffer(BufferType type);
	~GLBuffer();

	GLBuffer() = delete;
	GLBuffer(const GLBuffer&) = delete;
	GLBuffer& operator=(const GLBuffer&) = delete;

	void Bind() const;
	void Unbind() const;

	void WriteData(const void* data, size_t size, unsigned int usage) const;

private:
	unsigned int m_id;
	unsigned int m_target;
};

