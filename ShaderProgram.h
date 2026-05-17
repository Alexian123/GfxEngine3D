#pragma once

class ShaderProgram
{
public:
	ShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
	~ShaderProgram();

	void Bind() const;
	void Unbind() const;

	ShaderProgram() = delete;
	ShaderProgram(const ShaderProgram& other) = delete;
	ShaderProgram& operator=(const ShaderProgram& other) = delete;

private:
	unsigned int m_id;

	unsigned int CompileShader(unsigned int type, const char* source);
};

