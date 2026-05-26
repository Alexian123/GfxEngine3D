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
		std::string vertexShaderSource;
		std::string fragmentShaderSource;

		// read shader source code from files
		try {
			vertexShaderSource = PreprocessShader(vertexShaderPath);
			fragmentShaderSource = PreprocessShader(fragmentShaderPath);
		}
		catch (std::exception& e) {
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

	void ShaderProgram::SetUniform(const std::string& name, unsigned int value)
	{
		glUniform1ui(GetUniformLocation(name), value);
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

	std::string ShaderProgram::PreprocessShader(const std::string& path)
	{
		std::unordered_set<std::string> includedFiles;

		std::filesystem::path filePath = std::filesystem::canonical(path);
		std::string source = ReadFile(filePath.string());

		return ProcessIncludes(
			source,
			filePath.parent_path(),
			includedFiles
		);
	}

	std::string ShaderProgram::ReadFile(const std::string& path)
	{
		std::ifstream file(path);
		if (!file.is_open())
			throw std::runtime_error("Failed to open shader file: " + path);

		std::stringstream ss;
		ss << file.rdbuf();
		return ss.str();
	}

	std::string ShaderProgram::ProcessIncludes(
		const std::string& source, 
		const std::filesystem::path& currentDir, 
		std::unordered_set<std::string>& includedFiles
	)
	{
		std::stringstream input(source);
		std::stringstream output;

		std::string line;

		while (std::getline(input, line))
		{
			std::string trimmed = line;

			// left trim
			trimmed.erase(0, trimmed.find_first_not_of(" \t"));

			if (trimmed.rfind("#include", 0) == 0)
			{
				size_t firstQuote = line.find('"');
				size_t lastQuote = line.rfind('"');

				if (firstQuote == std::string::npos || lastQuote == std::string::npos || firstQuote == lastQuote)
					throw std::runtime_error("Invalid #include syntax: " + line);

				std::string includePath = line.substr(firstQuote + 1, lastQuote - firstQuote - 1);

				std::filesystem::path fullPath = currentDir / includePath;
				fullPath = std::filesystem::canonical(fullPath);

				std::string key = fullPath.string();

				if (includedFiles.find(key) != includedFiles.end())
					continue;

				includedFiles.insert(key);

				std::string includedSource = ReadFile(key);

				output << ProcessIncludes(
					includedSource,
					fullPath.parent_path(),
					includedFiles
				);
			}
			else
			{
				output << line << '\n';
			}
		}

		return output.str();
	}
}