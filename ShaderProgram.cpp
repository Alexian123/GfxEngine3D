#include "ShaderProgram.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace GfxEngine3D
{
	ShaderProgram::ShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath)
	{
		std::ifstream vertexShaderFile;
		std::ifstream fragmentShaderFile;
		std::string vertexShaderSource;
		std::string fragmentShaderSource;

		// ensure ifstream objects can throw exceptions
		vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		// read shader source code from files
		try {
			vertexShaderFile.open(vertexShaderPath);
			fragmentShaderFile.open(fragmentShaderPath);
			std::stringstream vertexShaderStream, fragmentShaderStream;
			vertexShaderStream << vertexShaderFile.rdbuf();
			fragmentShaderStream << fragmentShaderFile.rdbuf();
			vertexShaderFile.close();
			fragmentShaderFile.close();
			vertexShaderSource = vertexShaderStream.str();
			fragmentShaderSource = fragmentShaderStream.str();
		}
		catch (std::ifstream::failure& e) {
			std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n" << e.what() << std::endl;
			// TODO: use default hardcoded shaders
		}

		// compile shaders
		unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource.c_str());
		unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource.c_str());

		// link shaders into shader program
		m_id = glCreateProgram();
		glAttachShader(m_id, vertexShader);
		glAttachShader(m_id, fragmentShader);
		glLinkProgram(m_id);

		// check for linking errors
		int success = 0;
		char infoLog[512] = "";
		glGetProgramiv(m_id, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(m_id, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(m_id);
	}

	void ShaderProgram::Bind() const
	{
		glUseProgram(m_id);
	}

	void ShaderProgram::Unbind() const
	{
		glUseProgram(0);
	}

	void ShaderProgram::SetUniform(const std::string& name, float value)
	{
		glUniform1f(GetUniformLocation(name), value);
	}

	void ShaderProgram::SetUniform(const std::string& name, int value)
	{
		glUniform1i(GetUniformLocation(name), value);
	}

	void ShaderProgram::SetUniform(const std::string& name, bool value)
	{
		SetUniform(name, static_cast<int>(value));
	}

	void ShaderProgram::SetUniform(const std::string& name, const glm::vec3& value)
	{
		glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
	}

	void ShaderProgram::SetUniform(const std::string& name, const glm::mat4& value)
	{
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
	}

	void ShaderProgram::SetDefaultVertexAttribute(int index, float x, float y) const
	{
		glVertexAttrib2f(index, x, y);
	}

	void ShaderProgram::SetDefaultVertexAttribute(int index, float x, float y, float z) const
	{
		glVertexAttrib3f(index, x, y, z);
	}

	void ShaderProgram::SetDefaultVertexAttribute(int index, float x, float y, float z, float w) const
	{
		glVertexAttrib4f(index, x, y, z, w);
	}

	unsigned int ShaderProgram::CompileShader(unsigned int type, const char* source)
	{
		unsigned int shader = glCreateShader(type);
		glShaderSource(shader, 1, &source, NULL);
		glCompileShader(shader);

		// check for compilation errors
		int success = 0;
		static char infoLog[512] = "";
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::" << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT")
				<< "::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		return shader;
	}

	int ShaderProgram::GetUniformLocation(const std::string& name)
	{
		if (m_uniformLocationCache.find(name) == m_uniformLocationCache.end()) {
			m_uniformLocationCache[name] = glGetUniformLocation(m_id, name.c_str());
		}
		return m_uniformLocationCache[name];
	}
}