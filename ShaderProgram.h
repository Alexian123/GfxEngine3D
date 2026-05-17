#pragma once

#include <string>
#include <unordered_map>

class ShaderProgram
{
public:
	ShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath);
	~ShaderProgram();

	void Bind() const;
	void Unbind() const;

	void SetUniform(const std::string& name, float value);
	void SetUniform(const std::string& name, int value);
	void SetUniform(const std::string& name, bool value);

	ShaderProgram() = delete;
	ShaderProgram(const ShaderProgram& other) = delete;
	ShaderProgram& operator=(const ShaderProgram& other) = delete;

private:
	unsigned int m_id;
	std::unordered_map<std::string, int> m_uniformLocationCache;

	unsigned int CompileShader(unsigned int type, const char* source);
	int GetUniformLocation(const std::string& name);
};

