#pragma once

#include <vector>

class Mesh
{
public:
	Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
	~Mesh();

	void Bind() const;
	void Unbind() const;

	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

private:
	unsigned int m_VAO, m_VBO, m_EBO;
};

